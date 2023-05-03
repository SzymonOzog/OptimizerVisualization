#pragma once
#include "DataStructures.h"

class Shape
{
public:
    IndexedLineVector GetIndexedLineVector();
    IndexedTriangleVector GetIndexedTriangleVector();
    
    virtual Vec3 GetColor(int triangle_index);

    Vec3 position;
protected:
    IndexedLineVector indexedLineVector;
    IndexedTriangleVector indexedTriangleVector;

};

class Cube : public Shape
{
public:
    Cube(float size);

    virtual Vec3 GetColor(int triangle_index) override;

    std::vector<Vec3> colors;
};

class Plane : public Shape
{
public:
    Plane(int xSize, int zSize,float xLen=10.f, float zLen=10.f, bool twoSided = true);
};