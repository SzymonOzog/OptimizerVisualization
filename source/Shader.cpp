#include "Shader.h"
#include "Math.h"

Shader::Shader(const Vec3& ambientLight, const Vec3& directionalLightColor) : ambientLight(ambientLight), directionalLightColor(directionalLightColor)
{
}

void Shader::initFrame(const ViewInfo& viewInfo, const Mat4& worldViewProjection, const Vec3& actorPosition)
{
    this->worldViewProjection = worldViewProjection;
    this->actorPosition = actorPosition;
}

void Shader::transformVertex(Vertex& vertex)
{
    vertex.position = worldViewProjection * (vertex.position + actorPosition);
}

Vec3 Shader::shadePixel(const Vertex& vertex)
{
    float directionalLightAmount = std::max(0.f,Math::dotProduct(vertex.normal, Vec3({0.f,-1.f,0.f})));
    return Math::hadamard(vertex.color, ambientLight) + Math::hadamard(vertex.color, directionalLightColor) * directionalLightAmount;
}