#include "BufferController.h"
#include <cmath>

BufferController::BufferController(int width, int height)
{
    buffer = new Buffer();
    buffer->data = new Vec3[width * height];
    buffer->width = width;
    buffer->height = height;

    for (int i = 0; i < buffer->width * buffer->height; i++)
    {
        PutPixel(buffer, Point{ i % buffer->width, i / buffer->width }, Vec3{ 1.0f, 1.0f, 1.0f });
    }

    DrawLine(buffer, Point{ 0, 0 }, Point{ 100, 100 }, Vec3{ 0.0f, 0.0f, 0.0f });
    DrawLine(buffer, Point{ 0, 100 }, Point{ 100, 100 }, Vec3{ 0.0f, 1.0f, 0.0f });

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