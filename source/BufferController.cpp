#include "BufferController.h"
#include <iostream>
#include <cmath>

BufferController::BufferController(int width, int height)
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

    IndexedTriangleVector cube;
    float size = 0.5f;

    cube.vertices.push_back({-size,-size,-size}); // 0
	cube.vertices.push_back({size,-size,-size}); // 1
	cube.vertices.push_back({-size,size,-size}); // 2
	cube.vertices.push_back({size,size,-size}); // 3
	cube.vertices.push_back({-size,-size,size}); // 4
	cube.vertices.push_back({size,-size,size}); // 5
	cube.vertices.push_back({-size,size,size}); // 6
	cube.vertices.push_back({size,size,size}); // 7

    cube.indices = {
        0,2,1, 2,3,1,
        1,3,5, 3,7,5,
        2,6,3, 3,6,7,
        4,5,7, 4,7,6,
        0,4,2, 2,4,6,
        0,1,4, 1,5,4
    };

    std::vector<Vec3> colors{
        Color::Red, Color::Green, Color::Blue, Color::Yellow, Color::Cyan, Color::Magenta,
        Color::Red, Color::Green, Color::Blue, Color::Yellow, Color::Cyan, Color::Magenta
    };

    cube.projectedVertices.resize(cube.vertices.size());

    Mat3 rotation = Mat3::RotationZ(viewInfo.RotZ) * Mat3::RotationY(viewInfo.RotY) * Mat3::RotationX(viewInfo.RotX);
    for (int i = 0; i < cube.vertices.size(); i++)
    {
        cube.vertices[i] = rotation * cube.vertices[i];
        cube.vertices[i].z += 2.f;
        cube.projectedVertices[i] = ProjectToScreen(cube.vertices[i]);
    }

    for (int i = 0; i < cube.indices.size(); i += 3)
    {
        if(Math::DotProduct(Math::CrossProduct(cube.vertices[cube.indices[i + 1]] - cube.vertices[cube.indices[i]], cube.vertices[cube.indices[i + 2]] - cube.vertices[cube.indices[i]]), cube.vertices[cube.indices[i]])  <= 0)
        {
            DrawTriangle(&cube.projectedVertices[cube.indices[i]], &cube.projectedVertices[cube.indices[i + 1]], &cube.projectedVertices[cube.indices[i + 2]], colors[i / 3]);
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
