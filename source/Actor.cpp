#include "Actor.h"
#include "BufferController.h"
#include "Color.h"
#include "Shapes.h"
#include "Shader.h"
#include "Event.h"
#include <iostream>

Actor::Actor() : position({0.0f, 10.0f, 10.0f}), shader(std::make_shared<Shader>(Vec3{0.1f, 0.1f, 0.1f}, Vec3{0.85f, 0.85f, 1.0f}))
{
}

void Actor::tick(float deltaTime)
{
}

void Actor::initFrame(const ViewInfo& viewInfo, const Mat4& worldViewProjection)
{
    shader->initFrame(viewInfo, worldViewProjection, position);
    tick(viewInfo.deltaTime);
}

bool Actor::isVisible() const
{
    return bIsVisible;
}

IndexedTriangleVector& Actor::getIndexedTriangleVector()
{
    return shape->getIndexedTriangleVector();
}


Landscape::Landscape() : Actor()
{
    shape = std::make_unique<Plane>(80,80,50.f,50.f, true);
    shader = std::make_shared<LandscapeShader>(Vec3{0.1f, 0.1f, 0.1f}, Vec3{0.85f, 0.85f, 1.0f});
}

void Landscape::tick(float deltaTime)
{
    Actor::tick(deltaTime);
    shape->calculateNormals();
}

void Landscape::initFrame(const ViewInfo &viewInfo, const Mat4 &worldViewProjection)
{
    if(viewInfo.mouseLeft && gBufferController->getEditMode() == EditMode::Sculpt)
    {
        const std::vector<float>& alphas = std::static_pointer_cast<LandscapeShader>(shader)->alphas;
        for (int i = 0; i < shape->getIndexedTriangleVector().vertices.size(); i++)
        {
            shape->getIndexedTriangleVector().vertices[i].position -= Vec3{0.f, 0.01f, 0.f} * alphas[i] * viewInfo.deltaTime; 
        }
    }
    Actor::initFrame(viewInfo, worldViewProjection);
}

Visualizer::Visualizer() : Actor()
{
    shape = std::make_unique<Sphere>(20,20, 0.5f);
    shape->calculateNormals();
}

void Visualizer::tick(float deltaTime)
{
    Actor::tick(deltaTime);
}