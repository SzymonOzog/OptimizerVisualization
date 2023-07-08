#include "Shader.h"
#include "Math.h"
#include "Color.h"
#include "BufferController.h"
#include <iostream>

#define INVALID_VECTOR Vec3({std::numeric_limits<float>::max(),std::numeric_limits<float>::max(),std::numeric_limits<float>::max()})

Shader::Shader(const Vec3& ambientLight, const Vec3& directionalLightColor, int numVerts) : 
ambientLight(ambientLight), directionalLightColor(directionalLightColor), projectionCache(numVerts, INVALID_VECTOR)
{
}

void Shader::initFrame(const ViewInfo& viewInfo, const Mat4& worldViewProjection, const Vec3& actorPosition)
{
    this->worldViewProjection = worldViewProjection;
    this->actorPosition = actorPosition;
    projectionCache.assign(projectionCache.size(), INVALID_VECTOR);
}

void Shader::transformVertex(Vertex& vertex)
{
    vertex.position = worldViewProjection * (vertex.position + actorPosition);
}

void Shader::projectVertex(Vertex &vertex, int width, int height, int index)
{
    if(projectionCache[index] != INVALID_VECTOR)
    {
        vertex.position = projectionCache[index];
    }
    else
    {
        vertex.position = Vec3({(vertex.position.x / vertex.position.w + 1.0f) * 0.5f * width, (vertex.position.y / vertex.position.w + 1.0f) * 0.5f * height, vertex.position.z / vertex.position.w});
        projectionCache[index] = vertex.position;
    }
}

Vec3 Shader::shadePixel(const Vertex& vertex)
{
    float directionalLightAmount = std::max(0.f,Math::dotProduct(vertex.normal, Vec3({0.f,-1.f,0.f})));
    return Math::hadamard(vertex.color, ambientLight) + Math::hadamard(vertex.color, directionalLightColor) * directionalLightAmount;
}

LandscapeShader::LandscapeShader(const Vec3 &ambientLight, const Vec3 &directionalLightColor, int numVerts) : 
Shader(ambientLight, directionalLightColor, numVerts), 
sphereLocation({0.f,0.f,std::numeric_limits<float>::max()}), 
closestDistance(50.f),
sphereLocationVertexIndex(-1)
{
}

void LandscapeShader::initFrame(const ViewInfo &viewInfo, const Mat4 &worldViewProjection, const Vec3 &actorPosition)
{
    Shader::initFrame(viewInfo, worldViewProjection, actorPosition);
    mousePos = Point({viewInfo.mouseX, viewInfo.mouseY});
    radius = viewInfo.innerRadius;
    outerRadius = viewInfo.outerRadius;
    closestDistance = gBufferController->getEditMode() == EditMode::Sculpt ? 50.f : std::numeric_limits<float>::max();
    alphas.clear();
}

void LandscapeShader::transformVertex(Vertex &vertex)
{
    Shader::transformVertex(vertex);

    if(gBufferController->getEditMode() != EditMode::Sculpt)
    {
        return;
    }

    float dist = Math::distance(vertex.position, sphereLocation);
    float alpha = 1.f;
    if(dist < radius)
    {
        alpha = 0.f;
        vertex.color = Color::Green;   
    }
    else if(dist < outerRadius)
    {
        alpha = (dist - radius) / (outerRadius - radius);
        vertex.color = Math::lerp(Color::Green, vertex.color, alpha);
    }
    alphas.push_back(1.f - alpha);
}

void LandscapeShader::projectVertex(Vertex &vertex, int width, int height, int index)
{
    if(gBufferController->getEditMode() == EditMode::None)
    {
        Shader::projectVertex(vertex,width,height, index);
        return;
    }
    
    Vec3 transformedPos = vertex.position;
    Shader::projectVertex(vertex, width, height, index);
    float dist = Math::distance(vertex.position, Vec3({(float)mousePos.x, (float)mousePos.y, 0.f}), true);
    if(dist < closestDistance)
    {
        closestDistance = dist;
        sphereLocation = transformedPos;
        sphereLocationVertexIndex = index;
    }
}
