#include "DataStructures.h"
#include "Math.h"

float Vec3::length()
{
    return sqrt(x*x + y*y + z*z);
}

void Vec3::normalize()
{
    float len = length();
    x/=len;
    y/=len;
    z/=len;
}

Vec3 Vec3::operator += (const Vec3& other)
{
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
}
Vec3 Vec3::operator -= (const Vec3& other)
{
    x -= other.x;
    y -= other.y;
    z -= other.z;
    return *this;
}
Vec3 Vec3::operator *= (const float& other)
{
    x *= other;
    y *= other;
    z *= other;
    return *this;
}
Vec3 Vec3::operator /= (const float& other)
{
    x /= other;
    y /= other;
    z /= other;
    return *this;
}
Vec3 Vec3::operator + (const Vec3& other) const
{
    Vec3 result = *this;
    result += other;
    return result;
}
Vec3 Vec3::operator - (const Vec3& other) const
{
    Vec3 result = *this;
    result -= other;
    return result;
}
Vec3 Vec3::operator * (const float& other) const
{
    Vec3 result = *this;
    result *= other;
    return result;
}
Vec3 Vec3::operator / (const float& other) const
{
    Vec3 result = *this;
    result /= other;
    return result;
}


Mat3 Mat3::identity()
{
    Mat3 result;
    result.data[0][0] = 1.0f;
    result.data[1][1] = 1.0f;
    result.data[2][2] = 1.0f;
    return result;
}

Mat3 Mat3::rotationZ(float angle)
{
    Mat3 result;
    result.data[0][0] = cos(angle);
    result.data[0][1] = -sin(angle);
    result.data[1][0] = sin(angle);
    result.data[1][1] = cos(angle);
    result.data[2][2] = 1.0f;
    return result;
}

Mat3 Mat3::rotationY(float angle)
{
    Mat3 result;
    result.data[0][0] = cos(angle);
    result.data[0][2] = sin(angle);
    result.data[1][1] = 1.0f;
    result.data[2][0] = -sin(angle);
    result.data[2][2] = cos(angle);
    return result;
}

Mat3 Mat3::rotationX(float angle)
{
    Mat3 result;
    result.data[0][0] = 1.0f;
    result.data[1][1] = cos(angle);
    result.data[1][2] = -sin(angle);
    result.data[2][1] = sin(angle);
    result.data[2][2] = cos(angle);
    return result;
}


Vec3 Mat3::operator*(const Vec3& other)
{
    Vec3 result;
    result.x = data[0][0] * other.x + data[0][1] * other.y + data[0][2] * other.z;
    result.y = data[1][0] * other.x + data[1][1] * other.y + data[1][2] * other.z;
    result.z = data[2][0] * other.x + data[2][1] * other.y + data[2][2] * other.z;
    return result;
}

Mat3 Mat3::operator*(const Mat3& other)
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


Mat4 Mat4::identity()
{
    Mat4 result;
    result.data[0][0] = 1.0f;
    result.data[1][1] = 1.0f;
    result.data[2][2] = 1.0f;
    result.data[3][3] = 1.0f;
    return result;
}

Mat4 Mat4::projection(float fov, float aspectRatio, float nearPlane, float farPlane)
{
    Mat4 result;
    float FOVRad = 1.0f / tan(fov * 0.5f / 180.0f * Math::PI);
    result.data[0][0] = aspectRatio * FOVRad;
    result.data[1][1] = FOVRad;
    result.data[2][2] = farPlane / (farPlane - nearPlane);
    result.data[3][2] = (-farPlane * nearPlane) / (farPlane - nearPlane);
    result.data[2][3] = 1.0f;
    result.data[3][3] = 0.0f;
    return result;
}

Mat4 Mat4::translation(const Vec3& translation)
{
    Mat4 result = identity();
    result.data[3][0] = translation.x;
    result.data[3][1] = translation.y;
    result.data[3][2] = translation.z;
    return result;
}

Mat4 Mat4::rotationX(float angle)
{
    Mat4 result = identity();
    result.data[1][1] = cos(angle);
    result.data[1][2] = -sin(angle);
    result.data[2][1] = sin(angle);
    result.data[2][2] = cos(angle);
    return result;
}

Mat4 Mat4::rotationY(float angle)
{
    Mat4 result = identity();
    result.data[0][0] = cos(angle);
    result.data[0][2] = sin(angle);
    result.data[2][0] = -sin(angle);
    result.data[2][2] = cos(angle);
    return result;
}

Mat4 Mat4::rotationZ(float angle)
{
    Mat4 result = identity();
    result.data[0][0] = cos(angle);
    result.data[0][1] = -sin(angle);
    result.data[1][0] = sin(angle);
    result.data[1][1] = cos(angle);
    return result;
}

Mat4 Mat4::transpose()
{
    Mat4 result = identity();
    for (int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            result.data[i][j] = data[j][i];
        }
    }
    return result;
}

Vec4 Mat4::operator* (const Vec4& RHS)
{
    Vec4 result;
    result.x = data[0][0] * RHS.x + data[1][0] * RHS.y + data[2][0] * RHS.z + data[3][0] * RHS.w;
    result.y = data[0][1] * RHS.x + data[1][1] * RHS.y + data[2][1] * RHS.z + data[3][1] * RHS.w;
    result.z = data[0][2] * RHS.x + data[1][2] * RHS.y + data[2][2] * RHS.z + data[3][2] * RHS.w;
    result.w = data[0][3] * RHS.x + data[1][3] * RHS.y + data[2][3] * RHS.z + data[3][3] * RHS.w;
    return result;
}

Mat4 Mat4::operator * (const Mat4& RHS)
{
    Mat4 result;
    for (int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            result.data[i][j] = data[i][0] * RHS.data[0][j] + data[i][1] * RHS.data[1][j] + data[i][2] * RHS.data[2][j] + data[i][3] * RHS.data[3][j];
        }

    }
    return result;
}

Vertex Vertex::operator+=(const Vertex &other)
{
    this->color += other.color;
    this->normal += other.normal;
    this->position += other.position;
    return *this;
}

Vertex Vertex::operator-=(const Vertex &other)
{
    this->color -= other.color;
    this->normal -= other.normal;
    this->position -= other.position;
    return *this;
}

Vertex Vertex::operator*=(const float &other)
{
    this->color *= other;
    this->normal *= other;
    this->position *= other;
    return *this;
}

Vertex Vertex::operator/=(const float &other)
{
    this->color /= other;
    this->normal /= other;
    this->position /= other;
    return *this;
}

Vertex Vertex::operator+(const Vertex &other) const
{
    Vertex result = *this;
    result += other;
    return result;
}

Vertex Vertex::operator-(const Vertex &other) const
{
    Vertex result = *this;
    result -= other;
    return result;
}

Vertex Vertex::operator*(const float &other) const
{
    Vertex result = *this;
    result *= other;
    return result;
}

Vertex Vertex::operator/(const float &other) const
{
    Vertex result = *this;
    result /= other;
    return result;
}
