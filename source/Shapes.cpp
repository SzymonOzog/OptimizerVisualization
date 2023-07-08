#include "Shapes.h"
#include "Color.h"
#include "Math.h"

IndexedLineVector Shape::GetIndexedLineVector()
{
    return indexedLineVector;
}

IndexedTriangleVector &Shape::getIndexedTriangleVector()
{
    return indexedTriangleVector;
}

void Shape::calculateNormals()
{
    for (int j = 0; j < indexedTriangleVector.indices.size(); j += 3)
    {
        Vec3 v0 = indexedTriangleVector.vertices[indexedTriangleVector.indices[j]].position;
        Vec3 v1 = indexedTriangleVector.vertices[indexedTriangleVector.indices[j + 1]].position;
        Vec3 v2 = indexedTriangleVector.vertices[indexedTriangleVector.indices[j + 2]].position;
        Vec3 faceNormal = Math::crossProduct(v1 - v0, v2 - v0);
        faceNormal.normalize();
        indexedTriangleVector.vertices[indexedTriangleVector.indices[j]].normal += faceNormal;
        indexedTriangleVector.vertices[indexedTriangleVector.indices[j + 1]].normal += faceNormal;
        indexedTriangleVector.vertices[indexedTriangleVector.indices[j + 2]].normal += faceNormal;
    }
    for (int j = 0; j < indexedTriangleVector.vertices.size(); j++)
    {
        indexedTriangleVector.vertices[j].normal.normalize();
    }
}

Cube::Cube(float size)
{
    indexedTriangleVector.vertices.reserve(8);
    indexedTriangleVector.vertices[0].position = Vec4({-size, -size, -size, 1.f});
    indexedTriangleVector.vertices[1].position = Vec4({size, -size, -size, 1.f});
    indexedTriangleVector.vertices[2].position = Vec4({-size, size, -size, 1.f});
    indexedTriangleVector.vertices[3].position = Vec4({size, size, -size, 1.f});
    indexedTriangleVector.vertices[4].position = Vec4({-size, -size, size, 1.f});
    indexedTriangleVector.vertices[5].position = Vec4({size, -size, size, 1.f});
    indexedTriangleVector.vertices[6].position = Vec4({-size, size, size, 1.f});
    indexedTriangleVector.vertices[7].position = Vec4({size, size, size, 1.f});
    indexedTriangleVector.indices = {
        0, 2, 1, 2, 3, 1,
        1, 3, 5, 3, 7, 5,
        2, 6, 3, 3, 6, 7,
        4, 5, 7, 4, 7, 6,
        0, 4, 2, 2, 4, 6,
        0, 1, 4, 1, 5, 4};

    indexedLineVector.indices = {
        0, 1, 1, 3, 3, 2, 2, 0,
        4, 5, 5, 7, 7, 6, 6, 4,
        0, 4, 1, 5, 2, 6, 3, 7};
    indexedLineVector.projectedVertices.resize(indexedLineVector.vertices.size());

    std::vector<Vec3> colors = {
        Color::Red, Color::Green, Color::Blue, Color::Yellow, Color::Cyan, Color::Magenta,
        Color::Red, Color::Green, Color::Blue, Color::Yellow, Color::Cyan, Color::Magenta};
    for (int i = 0; i < indexedTriangleVector.vertices.size(); i++)
    {
        indexedTriangleVector.vertices[i].color = colors[i];
    }
    calculateNormals();
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
    calculateNormals();
}

Sphere::Sphere(int slices, int stacks, float radius, Vec3 color)
{
    indexedTriangleVector.vertices.reserve(slices * stacks);
    indexedTriangleVector.indices.reserve(slices * stacks * 6);

    indexedTriangleVector.vertices.push_back(Vec3{0.f, radius, 0.f});
    indexedTriangleVector.vertices.back().color = color;
    for(int i = 1; i<stacks; i++)
    {
        float phi = Math::PI * i / stacks;
        for(int j = 0; j<slices; j++)
        {
            float theta = 2 * Math::PI * j / slices;
            indexedTriangleVector.vertices.push_back(Vec3{radius * sinf(phi) * cosf(theta), radius * cosf(phi), radius * sinf(phi) * sinf(theta)});
            indexedTriangleVector.vertices.back().color = color;
        }
    }
    indexedTriangleVector.vertices.push_back(Vec3{0.f, -radius, 0.f});
    indexedTriangleVector.vertices.back().color = color;
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
}
