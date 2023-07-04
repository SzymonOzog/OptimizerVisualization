#include "PixelShader.h"
#include "Math.h"
PixelShader::PixelShader(const Vec3& ambientLight, const Vec3& directionalLightColor) : ambientLight(ambientLight), directionalLightColor(directionalLightColor)
{
}

Vec3 PixelShader::operator()(const Vertex& vertex)
{
    float directionalLightAmount = std::max(0.f,Math::dotProduct(vertex.normal, Vec3({0.f,-1.f,0.f})));
    return Math::hadamard(vertex.color, ambientLight) + Math::hadamard(vertex.color, directionalLightColor) * directionalLightAmount;
}