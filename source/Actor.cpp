#include "Actor.h"

Actor::Actor() : position({0.0f, 0.0f, 0.0f})
{

}

void Actor::tick(float deltaTime)
{

}

IndexedTriangleVector& Actor::GetIndexedTriangleVector()
{
    return shape->GetIndexedTriangleVector();
}

Landscape::Landscape() : Actor()
{
    shape = std::make_unique<Plane>(80,80,100.f,100.f, true);
    
}

void Landscape::tick(float deltaTime)
{
    Actor::tick(deltaTime);
    shape->CalculateNormals();
}
Visualizer::Visualizer() : Actor()
{
    shape = std::make_unique<Sphere>(20,20, 0.5f);
    shape->CalculateNormals();
}

void Visualizer::tick(float deltaTime)
{
    Actor::tick(deltaTime);
}