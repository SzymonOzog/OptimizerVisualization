#include "BufferController.h"
#include <iostream>
#include <cmath>

BufferController::BufferController(int width, int height)
{
    buffer = new Buffer();
    buffer->data = new Vec3[width * height];
    buffer->width = width;
    buffer->height = height;

    for (int i = 0; i < buffer->width * buffer->height; i++)
    {
        PutPixel(buffer, Point{ i % buffer->width, i / buffer->width }, Vec3{ 0.0f, 0.0f, 0.0f });
    }

    IndexedLineVector cube;
    float size = 0.5f;
    cube.vertices.push_back(Vec3{ -size, -size, -size });
    cube.vertices.push_back(Vec3{ size, -size, -size });
    cube.vertices.push_back(Vec3{ -size, size, -size });
    cube.vertices.push_back(Vec3{ size, size, -size });
    cube.vertices.push_back(Vec3{ -size, -size, size });
    cube.vertices.push_back(Vec3{ size, -size, size });
    cube.vertices.push_back(Vec3{ -size, size, size });
    cube.vertices.push_back(Vec3{ size, size, size });
    
    cube.indices.push_back(0);
    cube.indices.push_back(1);
    cube.indices.push_back(1);
    cube.indices.push_back(3);
    cube.indices.push_back(3);
    cube.indices.push_back(2);
    cube.indices.push_back(2);
    cube.indices.push_back(0);
    cube.indices.push_back(4);
    cube.indices.push_back(5);
    cube.indices.push_back(5);
    cube.indices.push_back(7);
    cube.indices.push_back(7);
    cube.indices.push_back(6);
    cube.indices.push_back(6);
    cube.indices.push_back(4);
    cube.indices.push_back(0);
    cube.indices.push_back(4);
    cube.indices.push_back(1);
    cube.indices.push_back(5);
    cube.indices.push_back(2);
    cube.indices.push_back(6);
    cube.indices.push_back(3);
    cube.indices.push_back(7);

    for (int i = 0; i < cube.indices.size(); i += 2)
    {
        DrawLine(buffer, ProjectToScreen(cube.vertices[cube.indices[i]]), ProjectToScreen(cube.vertices[cube.indices[i + 1]]), Vec3{ 1.0f, 1.0f, 1.0f });
    }

}

BufferController::~BufferController()
{
    delete[] buffer->data;
    delete buffer;
}

Buffer* BufferController::GetBuffer()
{
    return buffer;
}

Point BufferController::ProjectToScreen(const Vec3& vertex)
{
    return Point{ (int)((vertex.x + 1.0f) * 0.5f * buffer->width), (int)((vertex.y + 1.0f) * 0.5f * buffer->height) };
}

void BufferController::DrawLine(Buffer* buffer, Point a, Point b, Vec3 Color)
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
        PutPixel(buffer, Point{ (int)x, (int)y }, Color);
        x += xIncrement;
        y += yIncrement;
    }
}

void BufferController::PutPixel(Buffer* buffer, Point a, Vec3 Color)
{
    buffer->data[a.x + a.y * buffer->width] = Color;
}