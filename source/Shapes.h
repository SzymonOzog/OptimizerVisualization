#pragma once
#include "DataStructures.h"

class Shape
{
public:
    IndexedLineVector GetIndexedLineVector();
    IndexedTriangleVector GetIndexedTriangleVector();
    std::vector<Vec3> GetColors();

    Vec3 position;
protected:
    IndexedLineVector indexedLineVector;
    IndexedTriangleVector indexedTriangleVector;
    std::vector<Vec3> colors;

};

class Cube : public Shape
{
public:
    Cube(float size);
};

class Plane : public Shape
{
public:
    Plane(int xSize, int zSize);
};