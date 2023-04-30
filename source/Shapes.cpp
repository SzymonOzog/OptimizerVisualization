#include "Shapes.h"
#include "Color.h"


IndexedLineVector Shape::GetIndexedLineVector()
{
    return indexedLineVector;
}

IndexedTriangleVector Shape::GetIndexedTriangleVector()
{
    return indexedTriangleVector;
}

Vec3 Shape::GetColor(int triangle_index)
{
    return Color::White;
}


Cube::Cube(float size)
{
    indexedTriangleVector.vertices = {
        {-size,-size,-size}, // 0
        {size,-size,-size}, // 1
        {-size,size,-size}, // 2
        {size,size,-size}, // 3
        {-size,-size,size}, // 4
        {size,-size,size}, // 5
        {-size,size,size}, // 6
        {size,size,size} //7
    };
    indexedTriangleVector.indices = {
        0,2,1, 2,3,1,
        1,3,5, 3,7,5,
        2,6,3, 3,6,7,
        4,5,7, 4,7,6,
        0,4,2, 2,4,6,
        0,1,4, 1,5,4
    };
    indexedTriangleVector.projectedVertices.resize(indexedTriangleVector.vertices.size());

    indexedLineVector.vertices = indexedTriangleVector.vertices;
    indexedLineVector.indices = {
        0,1, 1,3, 3,2, 2,0,
        4,5, 5,7, 7,6, 6,4,
        0,4, 1,5, 2,6, 3,7
    };
    indexedLineVector.projectedVertices.resize(indexedLineVector.vertices.size());

    colors = {
        Color::Red, Color::Green, Color::Blue, Color::Yellow, Color::Cyan, Color::Magenta,
        Color::Red, Color::Green, Color::Blue, Color::Yellow, Color::Cyan, Color::Magenta
    };
}

Vec3 Cube::GetColor(int triangle_index)
{
    return colors[triangle_index];
}

Plane::Plane(int xSize, int ySize, bool twoSided)
{
    indexedTriangleVector.vertices.reserve(xSize*ySize);
    for(int x = 0; x<xSize; x++)
    {
        for(int y = 0; y<ySize; y++)
        {
            indexedTriangleVector.vertices.push_back(Vec3{(float)x,0.f,float(y)});
        }
    }
    indexedTriangleVector.indices.reserve((xSize-1)*(ySize-1)*6);

    for(int x = 0; x<xSize-1; x++)
    {
        for(int y = 0; y<ySize-1; y++)
        {
            indexedTriangleVector.indices.push_back(x*ySize+y);
            indexedTriangleVector.indices.push_back(x*ySize+y+1);
            indexedTriangleVector.indices.push_back((x+1)*ySize+y);

            indexedTriangleVector.indices.push_back((x+1)*ySize+y);
            indexedTriangleVector.indices.push_back(x*ySize+y+1);
            indexedTriangleVector.indices.push_back((x+1)*ySize+y+1);

            if(twoSided)
            {
                indexedTriangleVector.indices.push_back(x*ySize+y);
                indexedTriangleVector.indices.push_back((x+1)*ySize+y);
                indexedTriangleVector.indices.push_back(x*ySize+y+1);

                indexedTriangleVector.indices.push_back((x+1)*ySize+y);
                indexedTriangleVector.indices.push_back((x+1)*ySize+y+1);
                indexedTriangleVector.indices.push_back(x*ySize+y+1);
            }
        }
    }
    indexedTriangleVector.projectedVertices.resize(indexedTriangleVector.vertices.size());
}
