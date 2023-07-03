#include "Shapes.h"
#include "DataStructures.h"
#include <memory>

class Actor
{
public:
    Actor();

    virtual void tick(float deltaTime);

    IndexedTriangleVector& getIndexedTriangleVector();
    virtual Vec3 getColor(int index);
    
    Vec3 position;
    std::unique_ptr<Shape> shape;
};

class Landscape : public Actor
{
public:
    Landscape();

    virtual void tick(float deltaTime) override;

};

class Visualizer : public Actor
{
public:
    Visualizer();

    virtual void tick(float deltaTime) override;
};