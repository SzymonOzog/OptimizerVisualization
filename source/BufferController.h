#pragma once
#include "DataStructures.h"
#include <memory>
#include "Shapes.h"
#include "Actor.h"

class BufferController
{
public:
    BufferController(int width, int height);
    ~BufferController();

    void fillBuffer(const ViewInfo& viewInfo);
    Buffer* getBuffer();

private:
    void clearBuffer();

    Vec3 projectToScreen(const Vec3& vertex);
    Vec3 projectToScreen(const Vec4& vertex);

    bool isPointInsideTriangle(const Point& p, const Vec3& v0, const Vec3& v1, const Vec3& v2);

    void drawTriangle(Vertex* v0, Vertex* v1, Vertex* v2, std::shared_ptr<Shader> ps);
    void drawFlatTriangle(Vertex* v0, Vertex* v1, Vertex* v2, std::shared_ptr<Shader> ps);

    void drawLine(Point a, Point b, Vec3 Color);
    void putPixel(Point a, Vec3 Color, float z = 0.0f);

    Vec3 getColor(const Vec3& vertex, const Vec3& normal, Vec3 unlitColor);
    Buffer* buffer;
    std::vector<float> zBuffer;
    std::vector<std::unique_ptr<Actor>> actors;

    float nearPlane;
    float farPlane;
    float fov;
    Mat4 projectionMatrix;

    Mat4 cameraRotationInverse;
    Vec3 cameraPosition;
};