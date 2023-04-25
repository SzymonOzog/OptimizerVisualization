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

std::vector<Vec3> Shape::GetColors()
{
    return colors;
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
