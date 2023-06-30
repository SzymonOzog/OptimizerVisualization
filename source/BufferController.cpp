#include "BufferController.h"
#include <iostream>
#include <cmath>
#include <algorithm>
#include "Math.h"
#include "Color.h"
BufferController::BufferController(int width, int height) : zBuffer(width * height, std::numeric_limits<float>::max()),
ambientLight({0.1f,0.1f,0.1f}),
directionalLightColor({0.8f,0.85f,1.f}),
nearPlane(0.1f),
farPlane(1000.f),
fov(90.f),
projectionMatrix(Mat4::projection(fov, (float)height / (float)width, nearPlane, farPlane)),
cameraRotationInverse(Mat4::identity()),
cameraPosition({0.f,0.f,0.f})
{
    buffer = new Buffer();
    buffer->data = new Vec3[width * height];
    buffer->width = width;
    buffer->height = height;

    actors.push_back(std::move(std::make_unique<Landscape>()));
    actors.push_back(std::move(std::make_unique<Visualizer>()));
}

BufferController::~BufferController()
{
    delete[] buffer->data;
    delete buffer;
}

void BufferController::fillBuffer(const ViewInfo& viewInfo)
{
    clearBuffer();

    cameraRotationInverse = Mat4::rotationY(viewInfo.rotY) * Mat4::rotationX(viewInfo.rotX) * Mat4::rotationZ(viewInfo.rotZ);
    Mat4 WorldViewProjectionMatrix = Mat4::translation(cameraPosition) * cameraRotationInverse * projectionMatrix; 
    cameraPosition += cameraRotationInverse.transpose() * viewInfo.deltaPosition;

    for (auto& actor : actors)
    {        
        float closestVertexDist = std::numeric_limits<float>::max();
        radius = viewInfo.innerRadius;
        outerRadius = viewInfo.outerRadius;
        
        sphereLocation = Vec3{0.f,0.f,std::numeric_limits<float>::max()};
        actor->position = Vec3({0.f,10.f,10.f});
        actor->tick(viewInfo.deltaTime);
        IndexedTriangleVector& shape = actor->getIndexedTriangleVector();
        
        Mat3 rotation = Mat3::identity();

        for (int i = 0; i < shape.vertices.size(); i++)
        {
            shape.transformedVertices[i] = WorldViewProjectionMatrix * Vec4((rotation * shape.vertices[i]) + actor->position);
            shape.normals[i] = rotation * shape.normals[i];
            shape.projectedVertices[i] = projectToScreen(shape.transformedVertices[i]);
            const auto& vertex = shape.projectedVertices[i];
            float distFromMouse = sqrt(pow(vertex.x - viewInfo.mouseX,2) + pow(vertex.y - viewInfo.mouseY,2));
            if (distFromMouse < 50.f && distFromMouse < closestVertexDist)
            {  
                closestVertexDist = distFromMouse;
                sphereLocation = shape.transformedVertices[i];
            }
        }

        for(int i = 0; i<shape.vertices.size(); i++)
        {
            float dist = Math::distance(shape.transformedVertices[i], sphereLocation);
            if(viewInfo.mouseLeft && dist < radius)
            {
                shape.vertices[i] -= Vec3({0.f,0.01f,0.f}) * viewInfo.deltaTime;
            }
            else if(viewInfo.mouseLeft && dist < outerRadius)
            {
                float alpha = (Math::distance(shape.transformedVertices[i], sphereLocation) - radius) / (outerRadius - radius);
                shape.vertices[i] -= Vec3({0.f,0.01f *(1.f - alpha),0.f}) * viewInfo.deltaTime;
            }
        }

        for (int i = 0; i < shape.indices.size(); i += 3)
        {
            int size = shape.indices.size();
            Vec3 v0 = shape.transformedVertices[shape.indices[i]];
            Vec3 v1 = shape.transformedVertices[shape.indices[i + 1]];
            Vec3 v2 = shape.transformedVertices[shape.indices[i + 2]];
            Vec3 faceNormal = Math::crossProduct(v1 - v0, v2 - v0);
            faceNormal.normalize();
            bool isFacingCamera = Math::dotProduct(faceNormal, v0)  <= 0;
            bool isInFrontOfNearPlane = v0.z > nearPlane && v1.z > nearPlane && v2.z > nearPlane;
            if(isFacingCamera && isInFrontOfNearPlane)
            {
                Vec3 unlitColor = actor->shape->getColor(i / 3);
                float sphereDist = (Math::distance(sphereLocation, v0, true) + Math::distance(sphereLocation, v1, true) + Math::distance(sphereLocation, v2, true))/3.f;
                if (sphereDist < radius )  
                {
                   unlitColor = Color::Red;
                }
                else if(sphereDist < outerRadius)
                {
                    float alpha = (sphereDist - radius) / (outerRadius - radius);
                    unlitColor = Math::hadamard(unlitColor, Color::Red) * (1.f - alpha) + unlitColor * alpha ;
                } 
                drawTriangle(&shape.projectedVertices[shape.indices[i]],
                 &shape.projectedVertices[shape.indices[i + 1]], &shape.projectedVertices[shape.indices[i + 2]],
                   &shape.normals[shape.indices[i]], &shape.normals[shape.indices[i + 1]], &shape.normals[shape.indices[i + 2]], unlitColor);
            }

        }
    }

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

void BufferController::drawTriangle(Vec3 *v0, Vec3 *v1, Vec3 *v2, Vec3* n0, Vec3* n1, Vec3* n2, Vec3 Color)
{
    if(v0->y > v1->y)
    {
        std::swap(v0, v1);
        std::swap(n0, n1);

    }
    if(v0->y > v2->y) 
    {
        std::swap(v0, v2);
        std::swap(n0, n2);
    }
    if(v1->y > v2->y) 
    {
        std::swap(v1, v2);
        std::swap(n1, n2);
    }

    if(v0->y == v1->y) 
    {
        drawFlatTopTriangle(v0, v1, v2, n0, n1, n2, Color);
    }
    else if(v1->y == v2->y)
    {
        drawFlatBottomTriangle(v0, v1, v2, n0, n1, n2, Color);
    }
    else
    {
        Vec3 v3;
        v3.y = v1->y;

        float lerpAmount = (float)(v1->y - v0->y) / (float)(v2->y - v0->y);
        v3.x = v0->x +  lerpAmount * (v2->x - v0->x);
        v3.z = v0->z + lerpAmount * (v2->z - v0->z);
        Vec3 n3 = Math::lerp(*n0, *n2, lerpAmount);

        drawFlatBottomTriangle(v0, v1, &v3, n0, n1, &n3, Color);
        drawFlatTopTriangle(v1, &v3, v2, n1, &n3, n2, Color);
    }
}
 
void BufferController::drawFlatBottomTriangle(Vec3 *v0, Vec3 *v1, Vec3 *v2, Vec3* n0, Vec3* n1, Vec3* n2, Vec3 Color)
{
    if(v1->x > v2->x) 
    {
        std::swap(v1, v2);
        std::swap(n1, n2);
    }
    float invslope1 = (float)(v1->x - v0->x) / (float)(v1->y - v0->y);
    float invslope2 = (float)(v2->x - v0->x) / (float)(v2->y - v0->y);

    float zInvslope1 = (float)(v1->z - v0->z) / (float)(v1->y - v0->y);
    float zInvslope2 = (float)(v2->z - v0->z) / (float)(v2->y - v0->y);

    const int yStart = std::clamp((int)ceil(v0->y - 0.5f), 0, buffer->height);
    const int yEnd = std::clamp((int)ceil(v1->y - 0.5f), 0, buffer->height);

    for (int y = yStart; y < yEnd; y++)
    {
        const int xStart = std::clamp((int)ceil(v0->x + invslope1 * (y + 0.5f - v0->y) - 0.5f), 0, buffer->width);
        const int xEnd =   std::clamp((int)ceil(v0->x + invslope2 * (y + 0.5f - v0->y) - 0.5f), 0, buffer->width);
        
        const float zStart = v0->z + zInvslope1 * (y + 0.5f - v0->y);
        const float zEnd = v0->z + zInvslope2 * (y + 0.5f - v0->y);

        Vec3 nStart = Math::lerp(*n0, *n1, (float)(y - v0->y) / (float)(v1->y - v0->y));
        Vec3 nEnd = Math::lerp(*n0, *n2, (float)(y  - v0->y) / (float)(v2->y - v0->y));

        for (int x = xStart; x < xEnd; x++)
        {
            float lerpAmount = (float)(x - xStart) / (float)(xEnd - xStart);
            Vec3 lerpedNormal = Math::lerp(nStart, nEnd, lerpAmount);
            Vec3 LitColor = getColor(Vec3({(float)x, (float)y, 0.f}), lerpedNormal, Color);
            putPixel(Point{ x, y }, LitColor, zStart + (zEnd - zStart) * lerpAmount);
        }
    }
}

void BufferController::drawFlatTopTriangle(Vec3 *v0, Vec3 *v1, Vec3 *v2, Vec3* n0, Vec3* n1, Vec3* n2, Vec3 Color)
{
    if(v0->x > v1->x) 
    {
        std::swap(v0, v1);
        std::swap(n0, n1);
    }

    float invslope1 = (float)(v2->x - v0->x) / (float)(v2->y - v0->y);
    float invslope2 = (float)(v2->x - v1->x) / (float)(v2->y - v1->y);

    float zInvslope1 = (float)(v2->z - v0->z) / (float)(v2->y - v0->y);
    float zInvslope2 = (float)(v2->z - v1->z) / (float)(v2->y - v1->y);

    const int yStart = std::clamp((int)ceil(v0->y - 0.5f), 0, buffer->height);
    const int yEnd =   std::clamp((int)ceil(v2->y - 0.5f), 0, buffer->height);
    

    for (int y = yStart; y < yEnd; y++)
    {
        const int xStart = std::clamp((int)ceil(v0->x + invslope1 * (y + 0.5f - v0->y) - 0.5f), 0, buffer->width);
        const int xEnd =   std::clamp((int)ceil(v1->x + invslope2 * (y + 0.5f - v0->y) - 0.5f), 0, buffer->width);

        const float zStart = v0->z + zInvslope1 * (y - v0->y);
        const float zEnd = v1->z + zInvslope2 * (y - v0->y);

        Vec3 nStart = Math::lerp(*n0, *n2, (float)(y - v0->y) /(float) (v2->y - v0->y));
        Vec3 nEnd = Math::lerp(*n1, *n2, (float)(y - v1->y) /(float) (v2->y - v1->y));

        for (int x = xStart; x < xEnd; x++)
        {
            float lerpAmount = (float)(x - xStart) / (float)(xEnd - xStart);
            Vec3 lerpedNormal = Math::lerp(nStart, nEnd, lerpAmount);
            Vec3 LitColor = getColor(Vec3({(float)x, (float)y, 0.f}), lerpedNormal, Color);
            putPixel(Point{ x, y }, LitColor, zStart + (zEnd - zStart) * lerpAmount);
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

Vec3 BufferController::getColor(const Vec3 &vertex, const Vec3 &normal, Vec3 unlitColor)
{
    float directionalLightAmount = std::max(0.f,Math::dotProduct(normal, Vec3({0.f,-1.f,0.f})));
    return Math::hadamard(unlitColor, ambientLight) + Math::hadamard(unlitColor, directionalLightColor) * directionalLightAmount;
}
