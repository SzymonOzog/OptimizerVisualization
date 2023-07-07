#include "Actor.h"
#include "iostream"


Actor::Actor() : position({0.0f, 10.0f, 10.0f}), shader(std::make_shared<Shader>(Vec3{0.1f, 0.1f, 0.1f}, Vec3{0.85f, 0.85f, 1.0f}))
{
}

void Actor::tick(float deltaTime)
{
}

void Actor::initFrame(const ViewInfo& viewInfo, const Mat4& worldViewProjection)
{
    shader->initFrame(viewInfo, worldViewProjection, position);
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
Visualizer::Visualizer() : Actor()
{
    shape = std::make_unique<Sphere>(20,20, 0.5f);
    shape->calculateNormals();
}

void Visualizer::tick(float deltaTime)
{
    Actor::tick(deltaTime);
}