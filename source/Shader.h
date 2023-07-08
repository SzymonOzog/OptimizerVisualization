#pragma once
#include "DataStructures.h"

class Shader
{
public:
    Shader(const Vec3& ambientLight, const Vec3& directionalLightColor, int numVerts);
    virtual void initFrame(const ViewInfo& viewInfo, const Mat4& worldViewProjection, const Vec3& actorPosition);
    virtual void transformVertex(Vertex& vertex);
    virtual void projectVertex(Vertex& vertex, int width, int height, int index);
    virtual Vec3 shadePixel(const Vertex& vertex);
    
protected:
    std::vector<Vec3> projectionCache;

private:
    Mat4 worldViewProjection;
    Vec3 actorPosition;
    Vec3 ambientLight;
    Vec3 directionalLightColor;
};

class LandscapeShader : public Shader
{
public:
    LandscapeShader(const Vec3& ambientLight, const Vec3& directionalLightColor, int numVerts);
    virtual void initFrame(const ViewInfo& viewInfo, const Mat4& worldViewProjection, const Vec3& actorPosition) override;
    virtual void transformVertex(Vertex& vertex) override;
    virtual void projectVertex(Vertex& vertex, int width, int height, int index) override;

    std::vector<float> alphas;
    int sphereLocationVertexIndex;
private:
    Point mousePos;
    float radius;
    float outerRadius;
    Vec3 sphereLocation;
    float closestDistance;
};