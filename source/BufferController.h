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

    Vec3 ProjectToScreen(const Vec3& vertex);

    void DrawTriangle(Vec3* v0, Vec3* v1, Vec3* v2, Vec3 Color);
    void DrawFlatBottomTriangle(Vec3* v0, Vec3* v1, Vec3* v2, Vec3 Color);
    void DrawFlatTopTriangle(Vec3* v0, Vec3* v1, Vec3* v2, Vec3 Color);

    void DrawLine(Point a, Point b, Vec3 Color);
    void PutPixel(Point a, Vec3 Color, float z = 0.0f);

    Buffer* buffer;
    std::vector<float> zBuffer;
    
    Vec3 ambientLight;
    Vec3 directionalLightColor;
};