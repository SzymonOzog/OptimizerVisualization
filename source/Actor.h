#include "Shapes.h"
#include "DataStructures.h"
#include "Shader.h"
#include <memory>

class Actor
{
public:
    Actor();

    virtual void tick(float deltaTime);

    IndexedTriangleVector& getIndexedTriangleVector();
    virtual void initFrame(const ViewInfo& viewInfo, const Mat4& worldViewProjection);

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

};

class Visualizer : public Actor
{
public:
    Visualizer();

    virtual void tick(float deltaTime) override;
};