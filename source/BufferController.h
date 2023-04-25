#pragma once
#include "DataStructures.h"


class BufferController
{
public:
    BufferController(int width, int height);
    ~BufferController();

    void FillBuffer(const ViewInfo& viewInfo);
    Buffer* GetBuffer();

private:

    Point ProjectToScreen(const Vec3& vertex);

    void DrawTriangle(Point* v0, Point* v1, Point* v2, Vec3 Color);
    void DrawFlatBottomTriangle(Point* v0, Point* v1, Point* v2, Vec3 Color);
    void DrawFlatTopTriangle(Point* v0, Point* v1, Point* v2, Vec3 Color);

    void DrawLine(Point a, Point b, Vec3 Color);
    void PutPixel(Point a, Vec3 Color);

    Buffer* buffer;

};