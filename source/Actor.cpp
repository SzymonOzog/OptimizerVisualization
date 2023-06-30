#include "Actor.h"

Actor::Actor() : position({0.0f, 0.0f, 0.0f})
{

}

void Actor::tick(float deltaTime)
{

}

Landscape::Landscape() : Actor()
{
    shape = std::make_unique<Plane>(80,80,100.f,100.f, true);
    
}

void Landscape::tick(float deltaTime)
{
    Actor::tick(deltaTime);
}

Visualizer::Visualizer() : Actor()
{
    shape = std::make_unique<Sphere>(20,20, 0.5f);
}

void Visualizer::tick(float deltaTime)
{
    Actor::tick(deltaTime);
}