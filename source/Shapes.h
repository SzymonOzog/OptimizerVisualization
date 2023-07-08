#pragma once
#include "DataStructures.h"
#include "Color.h"

class Shape
{
public:
    IndexedLineVector GetIndexedLineVector();
    IndexedTriangleVector &getIndexedTriangleVector();

    void calculateNormals();

    Vec3 position;

protected:
    IndexedLineVector indexedLineVector;
    IndexedTriangleVector indexedTriangleVector;
};

class Cube : public Shape
{
public:
    Cube(float size);
};

class Plane : public Shape
{
public:
    Plane(int xSize, int zSize, float xLen = 10.f, float zLen = 10.f, bool twoSided = true);
};

class Sphere : public Shape
{
public:
    Sphere(int slices, int stacks, float radius = 1.f, Vec3 color = Color::Red);
};