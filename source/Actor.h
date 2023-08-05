#pragma once

#include "DataStructures.h"
#include <memory>

class Event;
class GetMousePositionWorldSpaceEvent;
class Shader;
class Shape;

class Actor
{
public:
    Actor();

    virtual void tick(float deltaTime);

    IndexedTriangleVector& getIndexedTriangleVector();
    virtual void initFrame(const ViewInfo& viewInfo, const Mat4& worldViewProjection);
    bool isVisible() const;
    virtual void handleEvent(std::shared_ptr<Event> e);
    

    bool bIsVisible;
    Vec3 position;
    std::unique_ptr<Shape> shape;
    std::shared_ptr<Shader> shader;
};

class Landscape : public Actor
{
public:
    Landscape();

    virtual void tick(float deltaTime) override;
    virtual void initFrame(const ViewInfo& viewInfo, const Mat4& worldViewProjection) override;
    virtual void handleEvent(std::shared_ptr<Event> e) override;
    Vec3 getSpherePositionGradient() const;
    void moveSpherePosition(const Vec3& delta);
    void init(float (*callback)(float, float));
    void colorize();
private:
    int currentSpherePositionIndex;
};

class Visualizer : public Actor
{
public:
    Visualizer();

    virtual void tick(float deltaTime) override;
    virtual void handleEvent(std::shared_ptr<Event> e) override;
};

class VisualizerMover : public Actor
{
public:
    VisualizerMover();

    virtual void initFrame(const ViewInfo& viewInfo, const Mat4& worldViewProjection) override;

private:
    std::weak_ptr<GetMousePositionWorldSpaceEvent> mousePositionEvent;
};