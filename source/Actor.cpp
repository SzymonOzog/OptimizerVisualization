#include "Actor.h"
#include "BufferController.h"
#include "Color.h"
#include "Shapes.h"
#include "Shader.h"
#include "Event.h"
#include <iostream>

Actor::Actor() : position({0.0f, 10.0f, 10.0f}), shader(std::make_shared<Shader>(Vec3{0.1f, 0.1f, 0.1f}, Vec3{0.85f, 0.85f, 1.0f}, 0))
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

void Actor::handleEvent(std::shared_ptr<Event> e)
{
}

IndexedTriangleVector& Actor::getIndexedTriangleVector()
{
    return shape->getIndexedTriangleVector();
}


Landscape::Landscape() : Actor()
{
    shape = std::make_unique<Plane>(80,80,50.f,50.f, true);
    shader = std::make_shared<LandscapeShader>(Vec3{0.1f, 0.1f, 0.1f}, Vec3{0.85f, 0.85f, 1.0f}, getIndexedTriangleVector().vertices.size());
}

void Landscape::tick(float deltaTime)
{
    Actor::tick(deltaTime);
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
        shape->calculateNormals();
    }
    Actor::initFrame(viewInfo, worldViewProjection);
}

void Landscape::handleEvent(std::shared_ptr<Event> e)
{
    if(auto getMousePositionWorldSpaceEvent = std::dynamic_pointer_cast<GetMousePositionWorldSpaceEvent>(e))
    {
        getMousePositionWorldSpaceEvent->mousePosition = getIndexedTriangleVector().vertices[std::static_pointer_cast<LandscapeShader>(shader)->sphereLocationVertexIndex].position + position;
    }
}

Visualizer::Visualizer() : Actor()
{
    shape = std::make_unique<Sphere>(20,20, 0.5f);
    shape->calculateNormals();
    //@TODO double shader creation - no me gusta
    shader = std::make_shared<Shader>(Vec3{0.1f, 0.1f, 0.1f}, Vec3{0.85f, 0.85f, 1.0f}, getIndexedTriangleVector().vertices.size());
}

void Visualizer::tick(float deltaTime)
{
    Actor::tick(deltaTime);
}

void Visualizer::handleEvent(std::shared_ptr<Event> e)
{
    if(auto setVisualiserPositionEvent = std::dynamic_pointer_cast<SetVisualiserPositionEvent>(e))
    {

        position = setVisualiserPositionEvent->position;
        setVisualiserPositionEvent->endEvent();    
    }
}

VisualizerMover::VisualizerMover() : Actor()
{
    shape = std::make_unique<Sphere>(20,20, 0.49f, Color::Magenta);
    shape->calculateNormals();
    shader = std::make_shared<Shader>(Vec3{0.1f, 0.1f, 0.1f}, Vec3{0.85f, 0.85f, 1.0f}, getIndexedTriangleVector().vertices.size());
}

void VisualizerMover::initFrame(const ViewInfo &viewInfo, const Mat4 &worldViewProjection)
{
    Actor::initFrame(viewInfo, worldViewProjection);
    bIsVisible = gBufferController->getEditMode() == EditMode::MoveSphere;
    if (bIsVisible && mousePositionEvent.expired())
    {
        auto sp = std::make_shared<GetMousePositionWorldSpaceEvent>();
        gBufferController->addEvent(sp);
        mousePositionEvent = sp;
    }
    else if (bIsVisible)
    {
        position = mousePositionEvent.lock()->mousePosition - Vec3{0.f, 0.5f, 0.f};
        if(viewInfo.mouseLeft)
        {
            gBufferController->addEvent(std::make_shared<SetVisualiserPositionEvent>(position));
        }
    }
    else if(!bIsVisible && !mousePositionEvent.expired())
    {
        mousePositionEvent.lock()->endEvent();
    }
}
