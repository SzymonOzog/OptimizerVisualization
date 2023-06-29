#include "Shapes.h"
#include "Color.h"
#include "Math.h"

IndexedLineVector Shape::GetIndexedLineVector()
{
    return indexedLineVector;
}

IndexedTriangleVector &Shape::GetIndexedTriangleVector()
{
    return indexedTriangleVector;
}

Vec3 Shape::GetColor(int triangle_index)
{
    return Color::White;
}

void Shape::CalculateNormals()
{
    indexedTriangleVector.normals = std::vector<Vec3>(indexedTriangleVector.vertices.size(), Vec3{0.f, 0.f, 0.f});

    for (int j = 0; j < indexedTriangleVector.indices.size(); j += 3)
    {

        Vec3 v0 = indexedTriangleVector.vertices[indexedTriangleVector.indices[j]];
        Vec3 v1 = indexedTriangleVector.vertices[indexedTriangleVector.indices[j + 1]];
        Vec3 v2 = indexedTriangleVector.vertices[indexedTriangleVector.indices[j + 2]];
        Vec3 faceNormal = Math::crossProduct(v1 - v0, v2 - v0);
        faceNormal.normalize();
        indexedTriangleVector.normals[indexedTriangleVector.indices[j]] += faceNormal;
        indexedTriangleVector.normals[indexedTriangleVector.indices[j + 1]] += faceNormal;
        indexedTriangleVector.normals[indexedTriangleVector.indices[j + 2]] += faceNormal;
    }
    for (int j = 0; j < indexedTriangleVector.normals.size(); j++)
    {
        indexedTriangleVector.normals[j].normalize();
    }
}

Cube::Cube(float size)
{
    indexedTriangleVector.vertices = {
        {-size, -size, -size}, // 0
        {size, -size, -size},  // 1
        {-size, size, -size},  // 2
        {size, size, -size},   // 3
        {-size, -size, size},  // 4
        {size, -size, size},   // 5
        {-size, size, size},   // 6
        {size, size, size}     // 7
    };
    indexedTriangleVector.indices = {
        0, 2, 1, 2, 3, 1,
        1, 3, 5, 3, 7, 5,
        2, 6, 3, 3, 6, 7,
        4, 5, 7, 4, 7, 6,
        0, 4, 2, 2, 4, 6,
        0, 1, 4, 1, 5, 4};
    indexedTriangleVector.projectedVertices.resize(indexedTriangleVector.vertices.size());
    indexedTriangleVector.transformedVertices.resize(indexedTriangleVector.vertices.size());

    indexedLineVector.vertices = indexedTriangleVector.vertices;
    indexedLineVector.indices = {
        0, 1, 1, 3, 3, 2, 2, 0,
        4, 5, 5, 7, 7, 6, 6, 4,
        0, 4, 1, 5, 2, 6, 3, 7};
    indexedLineVector.projectedVertices.resize(indexedLineVector.vertices.size());

    colors = {
        Color::Red, Color::Green, Color::Blue, Color::Yellow, Color::Cyan, Color::Magenta,
        Color::Red, Color::Green, Color::Blue, Color::Yellow, Color::Cyan, Color::Magenta};
    CalculateNormals();
}

Vec3 Cube::GetColor(int triangle_index)
{
    return colors[triangle_index];
}

Plane::Plane(int xSize, int zSize, float xLen, float zLen, bool twoSided)
{
    int numSides = twoSided ? 2 : 1;
    indexedTriangleVector.indices.reserve((xSize - 1) * (zSize - 1) * 6 * numSides);
    indexedTriangleVector.vertices.reserve(xSize * zSize * numSides);
    for (int i = 0; i < numSides; i++)
    {
        for (int x = 0; x < xSize; x++)
        {
            for (int z = 0; z < zSize; z++)
            {
                indexedTriangleVector.vertices.push_back(Vec3{(float)xLen * x / xSize, 0.f, (float)zLen * z / zSize});
            }
        }
    }
    for (int x = 0; x < xSize - 1; x++)
    {
        for (int z = 0; z < zSize - 1; z++)
        {
            indexedTriangleVector.indices.push_back(x * zSize + z);
            indexedTriangleVector.indices.push_back(x * zSize + z + 1);
            indexedTriangleVector.indices.push_back((x + 1) * zSize + z);

            indexedTriangleVector.indices.push_back((x + 1) * zSize + z);
            indexedTriangleVector.indices.push_back(x * zSize + z + 1);
            indexedTriangleVector.indices.push_back((x + 1) * zSize + z + 1);

            if (twoSided)
            {
                int offset = xSize * zSize;
                indexedTriangleVector.indices.push_back(offset + x * zSize + z);
                indexedTriangleVector.indices.push_back(offset + (x + 1) * zSize + z);
                indexedTriangleVector.indices.push_back(offset + x * zSize + z + 1);

                indexedTriangleVector.indices.push_back(offset + (x + 1) * zSize + z);
                indexedTriangleVector.indices.push_back(offset + (x + 1) * zSize + z + 1);
                indexedTriangleVector.indices.push_back(offset + x * zSize + z + 1);
            }
        }
    }

    indexedTriangleVector.projectedVertices.resize(indexedTriangleVector.vertices.size());
    indexedTriangleVector.transformedVertices.resize(indexedTriangleVector.vertices.size());

    CalculateNormals();
}

Sphere::Sphere(int slices, int stacks, float radius)
{
    indexedTriangleVector.vertices.reserve(slices * stacks);
    indexedTriangleVector.indices.reserve(slices * stacks * 6);

    indexedTriangleVector.vertices.push_back(Vec3{0.f, radius, 0.f});
    for(int i = 1; i<stacks; i++)
    {
        float phi = Math::PI * i / stacks;
        for(int j = 0; j<slices; j++)
        {
            float theta = 2 * Math::PI * j / slices;
            indexedTriangleVector.vertices.push_back(Vec3{radius * sin(phi) * cos(theta), radius * cos(phi), radius * sin(phi) * sin(theta)});
        }
    }
    indexedTriangleVector.vertices.push_back(Vec3{0.f, -radius, 0.f});

    for(int i = 0; i<slices; i++)
    {
        indexedTriangleVector.indices.push_back(0);
        indexedTriangleVector.indices.push_back((i + 1) % slices + 1);
        indexedTriangleVector.indices.push_back(i + 1);
    }

    for(int i = 0; i<stacks - 2; i++)
    {
        for(int j = 0; j<slices; j++)
        {
            indexedTriangleVector.indices.push_back(i * slices + j + 1);
            indexedTriangleVector.indices.push_back(i * slices + (j + 1) % slices + 1);
            indexedTriangleVector.indices.push_back((i + 1) * slices + j + 1);

            indexedTriangleVector.indices.push_back((i + 1) * slices + j + 1);
            indexedTriangleVector.indices.push_back(i * slices + (j + 1) % slices + 1);
            indexedTriangleVector.indices.push_back((i + 1) * slices + (j + 1) % slices + 1);
        }
    }

    for(int i = 0; i<slices; i++)
    {
        indexedTriangleVector.indices.push_back(indexedTriangleVector.vertices.size() - 1);
        indexedTriangleVector.indices.push_back(indexedTriangleVector.vertices.size() - 2 - (i + 1) % slices);
        indexedTriangleVector.indices.push_back(indexedTriangleVector.vertices.size() - 2 - i);
    }

    indexedTriangleVector.projectedVertices.resize(indexedTriangleVector.vertices.size());
    indexedTriangleVector.transformedVertices.resize(indexedTriangleVector.vertices.size());
}

Vec3 Sphere::GetColor(int triangle_index)
{
    return Color::Red;
}
