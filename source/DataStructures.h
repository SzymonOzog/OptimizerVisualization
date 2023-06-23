#pragma once
#include <vector>
#include <cmath>
#include "DataStructures.h"

struct Vec3 
{
    float x, y, z;

    float length()
    {
        return sqrt(x*x + y*y + z*z);
    }

    void normalize()
    {
        float len = length();
        x/=len;
        y/=len;
        z/=len;
    }

    Vec3 operator += (const Vec3& other)
    {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }
    Vec3 operator -= (const Vec3& other)
    {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }
    Vec3 operator *= (const float& other)
    {
        x *= other;
        y *= other;
        z *= other;
        return *this;
    }
    Vec3 operator /= (const float& other)
    {
        x /= other;
        y /= other;
        z /= other;
        return *this;
    }
    Vec3 operator + (const Vec3& other) const
    {
        Vec3 result = *this;
        result += other;
        return result;
    }
    Vec3 operator - (const Vec3& other) const
    {
        Vec3 result = *this;
        result -= other;
        return result;
    }
    Vec3 operator * (const float& other) const
    {
        Vec3 result = *this;
        result *= other;
        return result;
    }
    Vec3 operator / (const float& other) const
    {
        Vec3 result = *this;
        result /= other;
        return result;
    }


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

    Mat3()
    {
        data.resize(3);
        for (int i = 0; i < 3; i++)
        {
            data[i].resize(3);
            data[i] = { 0.0f, 0.0f, 0.0f };
        }
    }

    static Mat3 Identity()
    {
        Mat3 result;
        result.data[0][0] = 1.0f;
        result.data[1][1] = 1.0f;
        result.data[2][2] = 1.0f;
        return result;
    }

    static Mat3 RotationZ(float angle)
    {
        Mat3 result;
        result.data[0][0] = cos(angle);
        result.data[0][1] = -sin(angle);
        result.data[1][0] = sin(angle);
        result.data[1][1] = cos(angle);
        result.data[2][2] = 1.0f;
        return result;
    }

    static Mat3 RotationY(float angle)
    {
        Mat3 result;
        result.data[0][0] = cos(angle);
        result.data[0][2] = sin(angle);
        result.data[1][1] = 1.0f;
        result.data[2][0] = -sin(angle);
        result.data[2][2] = cos(angle);
        return result;
    }

    static Mat3 RotationX(float angle)
    {
        Mat3 result;
        result.data[0][0] = 1.0f;
        result.data[1][1] = cos(angle);
        result.data[1][2] = -sin(angle);
        result.data[2][1] = sin(angle);
        result.data[2][2] = cos(angle);
        return result;
    }


    Vec3 operator*(const Vec3& other)
    {
        Vec3 result;
        result.x = data[0][0] * other.x + data[0][1] * other.y + data[0][2] * other.z;
        result.y = data[1][0] * other.x + data[1][1] * other.y + data[1][2] * other.z;
        result.z = data[2][0] * other.x + data[2][1] * other.y + data[2][2] * other.z;
        return result;
    }

    Mat3 operator*(const Mat3& other)
    {
        Mat3 result;
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                result.data[i][j] = data[i][0] * other.data[0][j] + data[i][1] * other.data[1][j] + data[i][2] * other.data[2][j];
            }
        }
        return result;
    }
};

struct Mat4
{
    std::vector<std::vector<float>> data;

    Mat4() : data(4,  { 0.0f, 0.0f, 0.0f, 0.0f }) {}

    static Mat4 identity()
    {
        Mat4 result;
        result.data[0][0] = 1.0f;
        result.data[1][1] = 1.0f;
        result.data[2][2] = 1.0f;
        result.data[3][3] = 1.0f;
        return result;
    }

    static Mat4 projection(float FOV, float aspectRatio, float nearPlane, float farPlane)
    {
        Mat4 result;
        float FOVRad = 1.0f / tan(FOV * 0.5f / 180.0f * 3.14159f);
        result.data[0][0] = aspectRatio * FOVRad;
        result.data[1][1] = FOVRad;
        result.data[2][2] = farPlane / (farPlane - nearPlane);
        result.data[3][2] = (-farPlane * nearPlane) / (farPlane - nearPlane);
        result.data[2][3] = 1.0f;
        result.data[3][3] = 0.0f;
        return result;
    }

    Vec4 operator* (const Vec4& RHS)
    {
        Vec4 result;
        result.x = data[0][0] * RHS.x + data[0][1] * RHS.y + data[0][2] * RHS.z + data[0][3] * RHS.w;
        result.y = data[1][0] * RHS.x + data[1][1] * RHS.y + data[1][2] * RHS.z + data[1][3] * RHS.w;
        result.z = data[2][0] * RHS.x + data[2][1] * RHS.y + data[2][2] * RHS.z + data[2][3] * RHS.w;
        result.w = data[3][0] * RHS.x + data[3][1] * RHS.y + data[3][2] * RHS.z + data[3][3] * RHS.w;
        return result;
    }
};

struct Vertex
{
    Vec4 position;
    Vec3 normal;
    Vec3 color;
};

struct ViewInfo
{
    float rotX;
    float rotY;
    float rotZ;
    Vec3 position;

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
