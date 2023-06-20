#pragma once
#include "DataStructures.h"
#include <memory>
#include "Shapes.h"


class BufferController
{
public:
    BufferController(int width, int height);
    ~BufferController();

    void FillBuffer(const ViewInfo& viewInfo);
    Buffer* GetBuffer();

private:

    Vec3 ProjectToScreen(const Vec3& vertex);

    bool IsPointInsideTriangle(const Point& p, const Vec3& v0, const Vec3& v1, const Vec3& v2);

    void DrawTriangle(Vec3* v0, Vec3* v1, Vec3* v2, Vec3* n0, Vec3* n1, Vec3* n2, Vec3 Color);
    void DrawFlatBottomTriangle(Vec3* v0, Vec3* v1, Vec3* v2, Vec3* n0, Vec3* n1, Vec3* n2, Vec3 Color);
    void DrawFlatTopTriangle(Vec3* v0, Vec3* v1, Vec3* v2, Vec3* n0, Vec3* n1, Vec3* n2, Vec3 Color);

    void DrawLine(Point a, Point b, Vec3 Color);
    void PutPixel(Point a, Vec3 Color, float z = 0.0f);

    Vec3 GetColor(const Vec3& vertex, const Vec3& normal, Vec3 unlitColor);
    Buffer* buffer;
    std::vector<float> zBuffer;
    std::vector<std::unique_ptr<Shape>> shapes;

    Vec3 ambientLight;
    Vec3 directionalLightColor;


    Vec3 sphereLocation;
    float radius;
    float outerRadius;
};