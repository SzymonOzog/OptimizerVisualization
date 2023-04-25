#include "BufferController.h"
#include <iostream>
#include <cmath>
#include "Math.h"
#include "Color.h"
#include "Shapes.h"

BufferController::BufferController(int width, int height) : zBuffer(width * height, std::numeric_limits<float>::max())
{
    buffer = new Buffer();
    buffer->data = new Vec3[width * height];
    buffer->width = width;
    buffer->height = height;
}

BufferController::~BufferController()
{
    delete[] buffer->data;
    delete buffer;
}

void BufferController::FillBuffer(const ViewInfo& viewInfo)
{
    for (int i = 0; i < buffer->width * buffer->height; i++)
    {
        PutPixel(Point{ i % buffer->width, i / buffer->width }, Color::Black);
    }

    Shape cube1 = Cube(0.5f);
    Shape cube2 = Cube(0.5f);
    cube1.position = Vec3{ 0.0f, 0.0f, 3.0f };
    cube2.position = Vec3{ 1.0f, 0.0f, 2.0f };
    std::vector<Shape> shapes = { cube1, cube2 };
    for (auto& cube : shapes)
        {
        IndexedTriangleVector shape = cube.GetIndexedTriangleVector();
        std::vector<Vec3> colors = cube.GetColors();
        
        Mat3 rotation = Mat3::RotationZ(viewInfo.RotZ) * Mat3::RotationY(viewInfo.RotY) * Mat3::RotationX(viewInfo.RotX);
        for (int i = 0; i < shape.vertices.size(); i++)
        {
            shape.vertices[i] = rotation * shape.vertices[i];
            shape.vertices[i] += cube.position;
            shape.projectedVertices[i] = ProjectToScreen(shape.vertices[i]);
        }

        for (int i = 0; i < shape.indices.size(); i += 3)
        {
            Vec3 v0 = shape.vertices[shape.indices[i]];
            Vec3 v1 = shape.vertices[shape.indices[i + 1]];
            Vec3 v2 = shape.vertices[shape.indices[i + 2]];
            if(Math::DotProduct(Math::CrossProduct(v1 - v0, v2 - v0), v0)  <= 0)
            {
                DrawTriangle(&shape.projectedVertices[shape.indices[i]], &shape.projectedVertices[shape.indices[i + 1]], &shape.projectedVertices[shape.indices[i + 2]], colors[i / 3]);
            }
        }
    }
}

Buffer* BufferController::GetBuffer()
{
    return buffer;
}

Point BufferController::ProjectToScreen(const Vec3& vertex)
{
    return Point{ (int)((vertex.x / vertex.z + 1.0f) * 0.5f * buffer->width), (int)((vertex.y / vertex.z + 1.0f) * 0.5f * buffer->height) };
}

void BufferController::DrawTriangle(Point *v0, Point *v1, Point *v2, Vec3 Color)
{
    if(v0->y > v1->y) std::swap(v0, v1);
    if(v0->y > v2->y) std::swap(v0, v2);
    if(v1->y > v2->y) std::swap(v1, v2);

    if(v0->y == v1->y) 
    {
        DrawFlatTopTriangle(v0, v1, v2, Color);
    }
    else if(v1->y == v2->y)
    {
        DrawFlatBottomTriangle(v0, v1, v2, Color);
    }
    else
    {
        Point v3;
        v3.y = v1->y;
        v3.x = v0->x + (float)(v1->y - v0->y) / (float)(v2->y - v0->y) * (v2->x - v0->x);
        DrawFlatBottomTriangle(v0, v1, &v3, Color);
        DrawFlatTopTriangle(v1, &v3, v2, Color);
    }
}

void BufferController::DrawFlatBottomTriangle(Point *v0, Point *v1, Point *v2, Vec3 Color)
{
    if(v1->x > v2->x) std::swap(v1, v2);

    float invslope1 = (float)(v1->x - v0->x) / (float)(v1->y - v0->y);
    float invslope2 = (float)(v2->x - v0->x) / (float)(v2->y - v0->y);

    const int yStart = (int)ceil(v0->y - 0.5f);
    const int yEnd = (int)ceil(v1->y - 0.5f);
    

    for (int y = yStart; y < yEnd; y++)
    {
        const int xStart = (int)ceil(v0->x + invslope1 * (y + 0.5f - v0->y) - 0.5f);
        const int xEnd = (int)ceil(v0->x + invslope2 * (y + 0.5f - v0->y) - 0.5f);
        for (int x = xStart; x < xEnd; x++)
        {
            PutPixel(Point{ x, y }, Color);
        }
    }
}

void BufferController::DrawFlatTopTriangle(Point *v0, Point *v1, Point *v2, Vec3 Color)
{
    if(v0->x > v1->x) std::swap(v0, v1);

    float invslope1 = (float)(v2->x - v0->x) / (float)(v2->y - v0->y);
    float invslope2 = (float)(v2->x - v1->x) / (float)(v2->y - v1->y);

    const int yStart = (int)ceil(v0->y - 0.5f);
    const int yEnd = (int)ceil(v2->y - 0.5f);
    

    for (int y = yStart; y < yEnd; y++)
    {
        const int xStart = (int)ceil(v0->x + invslope1 * (y + 0.5f - v0->y) - 0.5f);
        const int xEnd = (int)ceil(v1->x + invslope2 * (y + 0.5f - v0->y) - 0.5f);
        for (int x = xStart; x < xEnd; x++)
        {
            PutPixel(Point{ x, y }, Color);
        }
    }
}

void BufferController::DrawLine(Point a, Point b, Vec3 Color)
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
        PutPixel(Point{ (int)x, (int)y }, Color);
        x += xIncrement;
        y += yIncrement;
    }
}

void BufferController::PutPixel(Point a, Vec3 Color)
{
    if (a.x < 0 || a.x >= buffer->width || a.y < 0 || a.y >= buffer->height)
    {
        return;
    }
    buffer->data[a.x + a.y * buffer->width] = Color;
}
