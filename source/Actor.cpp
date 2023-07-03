#include "Actor.h"

Actor::Actor() : position({0.0f, 0.0f, 0.0f})
{

}

void Actor::tick(float deltaTime)
{

}

IndexedTriangleVector& Actor::getIndexedTriangleVector()
{
    return shape->getIndexedTriangleVector();
}

Vec3 Actor::getColor(int index)
{
    return shape->getColor(index);
}

Landscape::Landscape() : Actor()
{
    shape = std::make_unique<Plane>(80,80,100.f,100.f, true);
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