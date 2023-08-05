#include "BufferController.h"
#include <iostream>
#include <cmath>
#include <algorithm>
#include "Math.h"
#include "Color.h"
#include "Shapes.h"
#include "Shader.h"
#include "Actor.h"
#include "Event.h"

BufferController* gBufferController = nullptr;

BufferController::BufferController(int width, int height) : zBuffer(width * height, std::numeric_limits<float>::max()),
nearPlane(0.01f),
farPlane(1000.f),
fov(90.f),
projectionMatrix(Mat4::projection(fov, (float)height / (float)width, nearPlane, farPlane)),
cameraRotationInverse(Mat4::identity()),
cameraPosition({0.f,0.f,0.f}),
editMode(EditMode::None)
{
    buffer = new Buffer();
    buffer->data = new Vec3[width * height];
    buffer->width = width;
    buffer->height = height;
    buffer->currentGradient = Vec3{0.0f, 0.0f, 0.0f};

    visualiserGradientEvent = std::make_shared<GetVisualiserGradientEvent>();
    addEvent(visualiserGradientEvent);
    visualiserPositionEvent = std::make_shared<GetVisualiserPositionEvent>();
    addEvent(visualiserPositionEvent);

    actors.push_back(std::move(std::make_unique<Landscape>()));
    actors.push_back(std::move(std::make_unique<Visualizer>()));
    actors.push_back(std::move(std::make_unique<VisualizerMover>()));


}

BufferController::~BufferController()
{
    delete[] buffer->data;
    delete buffer;
}

void BufferController::fillBuffer(const ViewInfo& viewInfo)
{
    clearBuffer();
    editMode = viewInfo.editMode;
    
    if(getEditMode() == EditMode::Run)
    {
        auto moveVisualiser = std::make_shared<MoveVisualiserEvent>(viewInfo.visualizerDelta);
        addEvent(moveVisualiser);
    }

    cameraRotationInverse = Mat4::rotationY(viewInfo.rotY) * Mat4::rotationX(viewInfo.rotX) * Mat4::rotationZ(viewInfo.rotZ);
    Mat4 WorldViewProjectionMatrix = Mat4::translation(cameraPosition) * cameraRotationInverse * projectionMatrix; 
    cameraPosition += cameraRotationInverse.transpose() * viewInfo.deltaPosition;

    for (auto& actor : actors)
    {
        actor->initFrame(viewInfo, WorldViewProjectionMatrix);
        for (int i = events.size() - 1; i >= 0; i--)
        {
            actor->handleEvent(events[i]);
            if (events[i]->bIsHandled)
            {
                events.erase(events.begin() + i);
            }
        }

        if(!actor->isVisible())
        {
            continue;
        }

        IndexedTriangleVector shape = actor->getIndexedTriangleVector();
        for (int i = 0; i < shape.vertices.size(); i++)
        {
            actor->shader->transformVertex(shape.vertices[i]);
        }

        for (int i = 0; i < shape.indices.size(); i += 3)
        {
            int size = shape.indices.size();
            Vertex v0 = shape.vertices[shape.indices[i]];
            Vertex v1 = shape.vertices[shape.indices[i + 1]];
            Vertex v2 = shape.vertices[shape.indices[i + 2]];
            Vec3 faceNormal = Math::crossProduct(v1.position - v0.position, v2.position - v0.position);
            faceNormal.normalize();
            bool isFacingCamera = Math::dotProduct(faceNormal, v0.position)  <= 0;
            bool isInFrontOfNearPlane = v0.position.z > nearPlane && v1.position.z > nearPlane && v2.position.z > nearPlane;
            if(isFacingCamera && isInFrontOfNearPlane)
            {
                actor->shader->projectVertex(v0, buffer->width, buffer->height, shape.indices[i]);
                actor->shader->projectVertex(v1, buffer->width, buffer->height, shape.indices[i+1]);
                actor->shader->projectVertex(v2, buffer->width, buffer->height, shape.indices[i+2]);
                drawTriangle(&v0, &v1, &v2, actor->shader);
            }
        }
    }

    buffer->currentGradient = visualiserGradientEvent->gradient;
    buffer->visualizerPosition = visualiserPositionEvent->position;
}

bool BufferController::isPointInsideTriangle(const Point& p, const Vec3& v0, const Vec3& v1, const Vec3& v2)
{
    int v0p_x = p.x - v0.x;
    int v0p_y = p.y - v0.y;

    bool p_v0v1 = (v1.x - v0.x) * v0p_y - (v1.y - v0.y) * v0p_x > 0;

    if (((v2.x - v0.x) * v0p_y - (v2.y - v0.y) * v0p_x > 0) == p_v0v1) 
        return false;
    if (((v2.x - v1.x) * (p.y - v1.y) - (v2.y - v1.y)*(p.x - v1.x) > 0 )!= p_v0v1) 
        return false;
    return true;
}

Buffer* BufferController::getBuffer()
{
    return buffer;
}

EditMode BufferController::getEditMode()
{
    return editMode;
}

void BufferController::addEvent(std::shared_ptr<Event> e)
{
    events.push_back(e);
}

void BufferController::initLandscape(float (*callback)(float, float))
{
    addEvent(std::make_shared<InitLandscapeEvent>(callback));
}

void BufferController::clearBuffer()
{
    for (int i = 0; i < buffer->width * buffer->height; i++)
    {
        putPixel(Point{ i % buffer->width, i / buffer->width }, Color::Black);
        zBuffer[i] = std::numeric_limits<float>::max();
    }
}

Vec3 BufferController::projectToScreen(const Vec3 &vertex)
{
    return Vec3{(vertex.x / vertex.z + 1.0f) * 0.5f * buffer->width, (vertex.y / vertex.z + 1.0f) * 0.5f * buffer->height, vertex.z };
}

Vec3 BufferController::projectToScreen(const Vec4 &vertex)
{
    return Vec3({(vertex.x / vertex.w + 1.0f) * 0.5f * buffer->width, (vertex.y / vertex.w + 1.0f) * 0.5f * buffer->height, vertex.z / vertex.w});
}

void BufferController::drawTriangle(Vertex* v0, Vertex* v1, Vertex* v2, std::shared_ptr<Shader> ps)
{
    if(v0->position.y > v1->position.y)
    {
        std::swap(v0, v1);
    }
    if(v0->position.y > v2->position.y)
    {
        std::swap(v0, v2);
    }
    if(v1->position.y > v2->position.y)
    {
        std::swap(v1, v2);
    }

    if(v0->position.y == v1->position.y) 
    {
        drawFlatTriangle(v2, v0, v1, ps);
    }
    else if(v1->position.y == v2->position.y)
    {
        drawFlatTriangle(v0, v1, v2, ps);
    }
    else
    {
        float lerpAmount = (float)(v1->position.y - v0->position.y) / (float)(v2->position.y - v0->position.y);
        Vertex v3 = Math::lerp(*v0, *v2, lerpAmount);

        drawFlatTriangle(v0, v1, &v3, ps);
        drawFlatTriangle(v2, v1, &v3, ps);
    }
}
 
void BufferController::drawFlatTriangle(Vertex* v0, Vertex* v1, Vertex* v2, std::shared_ptr<Shader> ps)
{
    if(v1->position.x > v2->position.x) 
    {
        std::swap(v1, v2);
    }

    float invslope1 = (float)(v1->position.x - v0->position.x) / (float)(v1->position.y - v0->position.y);
    float invslope2 = (float)(v2->position.x - v0->position.x) / (float)(v2->position.y - v0->position.y);

    int yStart = std::clamp((int)ceil(v0->position.y - 0.5f), 0, buffer->height);
    int yEnd =   std::clamp((int)ceil(v1->position.y - 0.5f), 0, buffer->height);

    if(yEnd < yStart)
    {
        std::swap(yStart, yEnd);
    }

    for (int y = yStart; y < yEnd; y++)
    {
        const int xStart = std::clamp((int)ceil(v0->position.x + invslope1 * (y + 0.5f - v0->position.y) - 0.5f), 0, buffer->width);
        const int xEnd =   std::clamp((int)ceil(v0->position.x + invslope2 * (y + 0.5f - v0->position.y) - 0.5f), 0, buffer->width);

        Vertex vStart = Math::lerp(*v0, *v1, (float)(y - v0->position.y) /(float) (v1->position.y - v0->position.y));
        Vertex vEnd = Math::lerp(*v0, *v2, (float)(y - v0->position.y) /(float) (v2->position.y - v0->position.y));

        for (int x = xStart; x < xEnd; x++)
        {
            float lerpAmount = (float)(x - xStart) / (float)(xEnd - xStart);
            //@TODO this lerps position for the second time, kinda wasteful maye make a templated compile time lerp?
            Vertex lerpedVert = Math::lerp(vStart, vEnd, lerpAmount);
            putPixel(Point{ x, y }, ps->shadePixel(lerpedVert), lerpedVert.position.z);
        }
    }
}

void BufferController::drawLine(Point a, Point b, Vec3 Color)
{
    int dx = b.x - a.x;
    int dy = b.y - a.y;
    int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);
    float xIncrement = dx / (float)steps;
    float yIncrement = dy / (float)steps;
    float x = a.x;
    float y = a.y;
    for (int i = 0; i <= steps; i++)
    {
        putPixel(Point{ (int)x, (int)y }, Color);
        x += xIncrement;
        y += yIncrement;
    }
}

void BufferController::putPixel(Point a, Vec3 Color, float z)
{
    if (z > zBuffer[a.x + a.y * buffer->width])
    {
        return;
    }
    zBuffer[a.x + a.y * buffer->width] = z;
    buffer->data[a.x + a.y * buffer->width] = Color;
}
