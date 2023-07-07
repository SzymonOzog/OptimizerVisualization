#include "Shader.h"
#include "Math.h"
#include "Color.h"

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

void Shader::projectVertex(Vertex &vertex, int width, int height)
{
    vertex.position = Vec3({(vertex.position.x / vertex.position.w + 1.0f) * 0.5f * width, (vertex.position.y / vertex.position.w + 1.0f) * 0.5f * height, vertex.position.z / vertex.position.w});
}

Vec3 Shader::shadePixel(const Vertex& vertex)
{
    float directionalLightAmount = std::max(0.f,Math::dotProduct(vertex.normal, Vec3({0.f,-1.f,0.f})));
    return Math::hadamard(vertex.color, ambientLight) + Math::hadamard(vertex.color, directionalLightColor) * directionalLightAmount;
}

LandscapeShader::LandscapeShader(const Vec3 &ambientLight, const Vec3 &directionalLightColor) : Shader(ambientLight, directionalLightColor), sphereLocation({0.f,0.f,std::numeric_limits<float>::max()}), closestDistance(50.f)
{
}

void LandscapeShader::initFrame(const ViewInfo &viewInfo, const Mat4 &worldViewProjection, const Vec3 &actorPosition)
{
    Shader::initFrame(viewInfo, worldViewProjection, actorPosition);
    mousePos = Point({viewInfo.mouseX, viewInfo.mouseY});
    radius = viewInfo.innerRadius;
    outerRadius = viewInfo.outerRadius;
    closestDistance = 50.f;
    alphas.clear();
}

void LandscapeShader::transformVertex(Vertex &vertex)
{
    Shader::transformVertex(vertex);
    float dist = Math::distance(vertex.position, sphereLocation);
    float alpha = 0.f;
    if(dist < radius)
    {
        alpha = 1.f;
        vertex.color = Color::Green;   
    }
    else if(dist < outerRadius)
    {
        alpha = (dist - radius) / (outerRadius - radius);
        vertex.color = Math::lerp(Color::Green, vertex.color, alpha);
    }
    alphas.push_back(alpha);
}

void LandscapeShader::projectVertex(Vertex &vertex, int width, int height)
{
    Vec3 transformedPos = vertex.position;
    Shader::projectVertex(vertex, width, height);
    float dist = Math::distance(vertex.position, Vec3({(float)mousePos.x, (float)mousePos.y, 0.f}), true);
    if(dist < closestDistance)
    {
        closestDistance = dist;
        sphereLocation = transformedPos;
    }
}
