#pragma once
#include <vector>
#include <cmath>


struct Vec3 
{
    float x, y, z;

    float length();
    void normalize();

    Vec3 operator += (const Vec3& other);
    Vec3 operator -= (const Vec3& other);
    Vec3 operator *= (const float& other);
    Vec3 operator /= (const float& other);
    Vec3 operator + (const Vec3& other) const;
    Vec3 operator - (const Vec3& other) const;
    Vec3 operator * (const float& other) const;
    Vec3 operator / (const float& other) const;


};

struct Vec4 : Vec3
{
    float w;

    Vec4() : Vec3({0.0f, 0.0f, 0.0f}), w(1.0f) {}
    Vec4(float x, float y, float z, float w) : Vec3({x, y, z}), w(w) {}
    Vec4(const Vec3& other) : Vec3(other), w(1.0f) {}
};

struct Mat3
{
    std::vector<std::vector<float>> data;

    Mat3() : data(3, { 0.0f, 0.0f, 0.0f }) {}

    static Mat3 identity();
    static Mat3 rotationZ(float angle);
    static Mat3 rotationY(float angle);
    static Mat3 rotationX(float angle);

    Vec3 operator*(const Vec3& other);
    Mat3 operator*(const Mat3& other);
};

struct Mat4
{
    std::vector<std::vector<float>> data;

    Mat4() : data(4,  { 0.0f, 0.0f, 0.0f, 0.0f }) {}

    static Mat4 identity();
    static Mat4 projection(float fov, float aspectRatio, float nearPlane, float farPlane);
    static Mat4 translation(const Vec3& translation);
    static Mat4 rotationX(float angle);
    static Mat4 rotationY(float angle);
    static Mat4 rotationZ(float angle);

    Mat4 transpose();
    Vec4 operator* (const Vec4& RHS);
    Mat4 operator * (const Mat4& RHS);
};

struct Vertex
{
    Vertex() : position({0.0f, 0.0f, 0.0f, 1.0f}), normal({0.0f, 0.0f, 0.0f}), color({1.0f, 1.0f, 1.0f}) {}
    Vertex(const Vec4& position, const Vec3& normal, const Vec3& color) : position(position), normal(normal), color(color) {}
    Vertex(const Vec4& position, const Vec3& normal) : position(position), normal(normal), color({1.0f, 1.0f, 1.0f}) {}
    Vertex(const Vec4& position) : position(position), normal({0.0f, 0.0f, 0.0f}), color({1.0f, 1.0f, 1.0f}) {}
    Vertex(const Vec3& position) : position({position.x, position.y, position.z, 1.0f}), normal({0.0f, 0.0f, 0.0f}), color({1.0f, 1.0f, 1.0f}) {}

    virtual Vertex operator += (const Vertex& other);
    virtual Vertex operator -= (const Vertex& other);
    virtual Vertex operator *= (const float& other);
    virtual Vertex operator /= (const float& other);
    virtual Vertex operator + (const Vertex& other) const;
    virtual Vertex operator - (const Vertex& other) const;
    virtual Vertex operator * (const float& other) const;
    virtual Vertex operator / (const float& other) const;
    
    Vec4 position;
    Vec3 normal;
    Vec3 color;
};

struct ViewInfo
{
    float rotX;
    float rotY;
    float rotZ;
    Vec3 deltaPosition;

    int mouseX;
    int mouseY;
    bool mouseLeft;

    int deltaTime;

    float innerRadius;
    float outerRadius;
};

struct Buffer 
{
    Vec3* data;
    int width, height;
};

struct Point
{
    int x, y;
};


struct IndexedLineVector
{
    std::vector<Vec3> vertices;
    std::vector<Vec3> projectedVertices;
    std::vector<int> indices;
};

struct IndexedTriangleVector
{
    std::vector<Vec3> vertices;
    std::vector<Vec4> transformedVertices;
    std::vector<Vec3> projectedVertices;
    std::vector<Vec3> normals;
    std::vector<int> indices;
};
