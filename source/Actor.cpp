#include "Actor.h"
#include "BufferController.h"
#include "Color.h"
#include "Shapes.h"
#include "Shader.h"
#include "Event.h"
#include "Math.h"
#include <iostream>
#include <algorithm>

Actor::Actor() : position({0.0f, 10.0f, 10.0f}), shader(std::make_shared<Shader>(Vec3{0.1f, 0.1f, 0.1f}, Vec3{0.85f, 0.85f, 1.0f}, 0)), bIsVisible(true)
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


Landscape::Landscape() : Actor(), currentSpherePositionIndex(0)
{
    shape = std::make_unique<Plane>(180,180,50.f,50.f, true);
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
        colorize(); 
        gBufferController->addEvent(std::make_shared<SetVisualiserPositionEvent>(shape->getIndexedTriangleVector().vertices[currentSpherePositionIndex].position + position));
    }
    Actor::initFrame(viewInfo, worldViewProjection);
}

void Landscape::handleEvent(std::shared_ptr<Event> e)
{
    if(auto getMousePositionWorldSpaceEvent = std::dynamic_pointer_cast<GetMousePositionWorldSpaceEvent>(e))
    {
        currentSpherePositionIndex = std::static_pointer_cast<LandscapeShader>(shader)->sphereLocationVertexIndex;
        getMousePositionWorldSpaceEvent->mousePosition = getIndexedTriangleVector().vertices[currentSpherePositionIndex].position + position;
    }
    else if (auto getVisualiserGradientEvent = std::dynamic_pointer_cast<GetVisualiserGradientEvent>(e))
    {
        getVisualiserGradientEvent->gradient = getSpherePositionGradient();
    }
    else if (auto moveVisualiserEvent = std::dynamic_pointer_cast<MoveVisualiserEvent>(e))
    {
        moveSpherePosition(moveVisualiserEvent->delta);
        moveVisualiserEvent->endEvent();
    }
    else if (auto getVisualizerPositionEvent = std::dynamic_pointer_cast<GetVisualiserPositionEvent>(e))
    {
        getVisualizerPositionEvent->position = getIndexedTriangleVector().vertices[currentSpherePositionIndex].position + position;
    }
    else if (auto initializeLandscapeEvent = std::dynamic_pointer_cast<InitLandscapeEvent>(e))
    {
        init(initializeLandscapeEvent->callback);
        initializeLandscapeEvent->endEvent();
    }
}

Vec3 Landscape::getSpherePositionGradient() const
{
    Vec3 gradient{0.f, 0.f, 0.f};
    const auto& indices = shape->getIndexedTriangleVector().indices;
    for(int i = 0; i < indices.size(); i+=3)
    {
        if(indices[i] == currentSpherePositionIndex || indices[i+1] == currentSpherePositionIndex || indices[i+2] == currentSpherePositionIndex)
        {
            Vec3 v0 = shape->getIndexedTriangleVector().vertices[indices[i]].position;
            Vec3 v1 = shape->getIndexedTriangleVector().vertices[indices[i+1]].position;
            Vec3 v2 = shape->getIndexedTriangleVector().vertices[indices[i+2]].position;
            if (v1.x == v0.x && v2.x != v0.x)
            {
                float dy = v2.y - v0.y;
                float dx = v2.x - v0.x;
                gradient.x -= dy/dx;
            }
            else if (v2.x == v0.x && v1.x != v0.x)
            {
                float dy = v1.y - v0.y;
                float dx = v1.x - v0.x;
                gradient.x -= dy/dx;
            }
            
            if (v1.z == v0.z && v2.z != v0.z)
            {
                float dy = v2.y - v0.y;
                float dz = v2.z - v0.z;
                gradient.z -= dy/dz;
            }
            else if (v2.z == v0.z && v1.z != v0.z)
            {
                float dy = v1.y - v0.y;
                float dz = v1.z - v0.z;
                gradient.z -= dy/dz;
            }
        }
    }
    return gradient;
}

void Landscape::moveSpherePosition(const Vec3 &delta)
{
    Vec3 NewPositionWS = shape->getIndexedTriangleVector().vertices[currentSpherePositionIndex].position + delta;
    float closestDistance = std::numeric_limits<float>::max();
    Vec3 closestVertex;
    for(int i = 0; i< shape->getIndexedTriangleVector().vertices.size(); i++)
    {
        const auto& v = getIndexedTriangleVector().vertices[i];
        float distance = Math::distance(v.position, NewPositionWS);
        if(distance < closestDistance)
        {
            closestDistance = distance;
            closestVertex = v.position;
            currentSpherePositionIndex = i;
        }
    }
    gBufferController->addEvent(std::make_shared<SetVisualiserPositionEvent>(closestVertex + position));
}

void Landscape::init(float (*callback)(float, float))
{
    IndexedTriangleVector& indexedTriangleVector = shape->getIndexedTriangleVector();
    for (auto & vertex : indexedTriangleVector.vertices)
    {
       vertex.position.y = callback(vertex.position.x, vertex.position.z);
    }
    shape->calculateNormals();
    colorize();
    gBufferController->addEvent(std::make_shared<SetVisualiserPositionEvent>(shape->getIndexedTriangleVector().vertices[currentSpherePositionIndex].position + position));
}

void Landscape::colorize()
{
    float maxY = std::max_element(shape->getIndexedTriangleVector().vertices.begin(), shape->getIndexedTriangleVector().vertices.end(), [](const Vertex& v1, const Vertex& v2){return v1.position.y < v2.position.y;})->position.y;
    float minY = std::min_element(shape->getIndexedTriangleVector().vertices.begin(), shape->getIndexedTriangleVector().vertices.end(), [](const Vertex& v1, const Vertex& v2){return v1.position.y < v2.position.y;})->position.y;
    float range = maxY - minY;
    for (auto & vertex : shape->getIndexedTriangleVector().vertices)
    {
        float lerpAmount = (vertex.position.y - minY) / range;
        vertex.color = Math::lerp(Color::Red, Color::Blue, lerpAmount);
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
        position = setVisualiserPositionEvent->position - Vec3{0.f, 0.5f, 0.f};
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
            gBufferController->addEvent(std::make_shared<SetVisualiserPositionEvent>(mousePositionEvent.lock()->mousePosition));
        }
    }
    else if(!bIsVisible && !mousePositionEvent.expired())
    {
        mousePositionEvent.lock()->endEvent();
    }
}
