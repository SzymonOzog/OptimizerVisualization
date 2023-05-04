#include "BufferController.h"
#include <iostream>
#include <cmath>
#include "Math.h"
#include "Color.h"

BufferController::BufferController(int width, int height) : zBuffer(width * height, std::numeric_limits<float>::max()),
ambientLight({0.1f,0.1f,0.1f}),
directionalLightColor({0.8f,0.85f,1.f})
{
    buffer = new Buffer();
    buffer->data = new Vec3[width * height];
    buffer->width = width;
    buffer->height = height;

    std::unique_ptr<Shape> plane = std::make_unique<Plane>(50,50);    
    shapes.push_back(std::move(plane));
}

BufferController::~BufferController()
{
    delete[] buffer->data;
    delete buffer;
}

void BufferController::FillBuffer(const ViewInfo& viewInfo)
{
    for (int i = 0; i < buffer->width * buffer->height; i++)
    {
        PutPixel(Point{ i % buffer->width, i / buffer->width }, Color::Black);
        zBuffer[i] = std::numeric_limits<float>::max();
    }

    for (auto& cube : shapes)
    {        
        float closestVertexDist = std::numeric_limits<float>::max();
        Vec3 sphereLocation;
        float radius = 2.f;
        float outerRadius = 3.5f;
        cube->position = viewInfo.position;
        IndexedTriangleVector& shape = cube->GetIndexedTriangleVector();
        
        Mat3 rotation = Mat3::RotationZ(viewInfo.rotZ) * Mat3::RotationY(viewInfo.rotY) * Mat3::RotationX(viewInfo.rotX);
        for (int i = 0; i < shape.vertices.size(); i++)
        {
            shape.projectedVertices[i] = ProjectToScreen((rotation * shape.vertices[i]) + cube->position);
            const auto& vertex = shape.projectedVertices[i];
            float distFromMouse = sqrt(pow(vertex.x - viewInfo.mouseX,2) + pow(vertex.y - viewInfo.mouseY,2));
            if (distFromMouse < 2.f && distFromMouse < closestVertexDist)
            {  
                closestVertexDist = distFromMouse;
                sphereLocation = shape.vertices[i];
            }
        }

        for(int i = 0; i<shape.vertices.size(); i++)
        {
            if(viewInfo.mouseLeft && Math::Distance(shape.vertices[i], sphereLocation) < radius)
            {
                shape.vertices[i] -= Vec3({0.f,0.01f,0.f}) * viewInfo.deltaTime;
            }
        }

        for (int i = 0; i < shape.indices.size(); i += 3)
        {
            Vec3 v0 = shape.vertices[shape.indices[i]];
            Vec3 v1 = shape.vertices[shape.indices[i + 1]];
            Vec3 v2 = shape.vertices[shape.indices[i + 2]];
            Vec3 faceNormal = Math::CrossProduct(v1 - v0, v2 - v0);
            faceNormal.normalize();
            
            if(Math::DotProduct(faceNormal, v0)  <= 0)
            {
                float directionalLightAmount = std::max(0.f,Math::DotProduct(faceNormal, Vec3({0.f,-1.f,0.f})));
                Vec3 unlitColor = cube->GetColor(i / 3);
                Vec3 litColor = Math::Hadamard(unlitColor, ambientLight) + Math::Hadamard(unlitColor, directionalLightColor) * directionalLightAmount;
                
                float sphereDist = (Math::Distance(sphereLocation, v0) + Math::Distance(sphereLocation, v1) + Math::Distance(sphereLocation, v2))/3.f;
                if (sphereDist < radius )  
                {
                   litColor = Color::Red;
                }
                else if(sphereDist < outerRadius)
                {
                    float alpha = (sphereDist - radius) / (outerRadius - radius);
                    litColor = Math::Hadamard(litColor, Color::Red) * (1.f - alpha) + litColor * alpha ;
                } 
                DrawTriangle(&shape.projectedVertices[shape.indices[i]],
                 &shape.projectedVertices[shape.indices[i + 1]], &shape.projectedVertices[shape.indices[i + 2]], litColor);
            }

        }
    }

}

bool BufferController::IsPointInsideTriangle(const Point& p, const Vec3& v0, const Vec3& v1, const Vec3& v2)
{
    int v0p_x = p.x - v0.x;
    int v0p_y = p.y - v0.y;

    bool p_v0v1 = (v1.x - v0.x) * v0p_y - (v1.y - v0.y) * v0p_x > 0;

    if (((v2.x - v0.x) * v0p_y - (v2.y - v0.y) * v0p_x > 0) == p_v0v1) 
        return false;
    if (((v2.x - v1.x) * (p.y - v1.y) - (v2.y - v1.y)*(p.x - v1.x) > 0 )!= p_v0v1) 
        return false;
    return true;
}

Buffer* BufferController::GetBuffer()
{
    return buffer;
}

Vec3 BufferController::ProjectToScreen(const Vec3& vertex)
{
    return Vec3{(vertex.x / vertex.z + 1.0f) * 0.5f * buffer->width, (vertex.y / vertex.z + 1.0f) * 0.5f * buffer->height, vertex.z };
}

void BufferController::DrawTriangle(Vec3 *v0, Vec3 *v1, Vec3 *v2, Vec3 Color)
{
    if(v0->y > v1->y) std::swap(v0, v1);
    if(v0->y > v2->y) std::swap(v0, v2);
    if(v1->y > v2->y) std::swap(v1, v2);

    if(v0->y == v1->y) 
    {
        DrawFlatTopTriangle(v0, v1, v2, Color);
    }
    else if(v1->y == v2->y)
    {
        DrawFlatBottomTriangle(v0, v1, v2, Color);
    }
    else
    {
        Vec3 v3;
        v3.y = v1->y;
        v3.x = v0->x + (float)(v1->y - v0->y) / (float)(v2->y - v0->y) * (v2->x - v0->x);
        v3.z = v0->z + (float)(v1->y - v0->y) / (float)(v2->y - v0->y) * (v2->z - v0->z);
        DrawFlatBottomTriangle(v0, v1, &v3, Color);
        DrawFlatTopTriangle(v1, &v3, v2, Color);
    }
}

void BufferController::DrawFlatBottomTriangle(Vec3 *v0, Vec3 *v1, Vec3 *v2, Vec3 Color)
{
    if(v1->x > v2->x) std::swap(v1, v2);

    float invslope1 = (float)(v1->x - v0->x) / (float)(v1->y - v0->y);
    float invslope2 = (float)(v2->x - v0->x) / (float)(v2->y - v0->y);

    float zInvslope1 = (float)(v1->z - v0->z) / (float)(v1->y - v0->y);
    float zInvslope2 = (float)(v2->z - v0->z) / (float)(v2->y - v0->y);

    const int yStart = (int)ceil(v0->y - 0.5f);
    const int yEnd = (int)ceil(v1->y - 0.5f);

    for (int y = yStart; y < yEnd; y++)
    {
        const int xStart = (int)ceil(v0->x + invslope1 * (y + 0.5f - v0->y) - 0.5f);
        const int xEnd = (int)ceil(v0->x + invslope2 * (y + 0.5f - v0->y) - 0.5f);
        
        const float zStart = v0->z + zInvslope1 * (y + 0.5f - v0->y);
        const float zEnd = v0->z + zInvslope2 * (y + 0.5f - v0->y);
        for (int x = xStart; x < xEnd; x++)
        {
            PutPixel(Point{ x, y }, Color, zStart + (zEnd - zStart) * (x + 0.5f - xStart) / (xEnd - xStart));
        }
    }
}

void BufferController::DrawFlatTopTriangle(Vec3 *v0, Vec3 *v1, Vec3 *v2, Vec3 Color)
{
    if(v0->x > v1->x) std::swap(v0, v1);

    float invslope1 = (float)(v2->x - v0->x) / (float)(v2->y - v0->y);
    float invslope2 = (float)(v2->x - v1->x) / (float)(v2->y - v1->y);

    float zInvslope1 = (float)(v2->z - v0->z) / (float)(v2->y - v0->y);
    float zInvslope2 = (float)(v2->z - v1->z) / (float)(v2->y - v1->y);

    const int yStart = (int)ceil(v0->y - 0.5f);
    const int yEnd = (int)ceil(v2->y - 0.5f);
    

    for (int y = yStart; y < yEnd; y++)
    {
        const int xStart = (int)ceil(v0->x + invslope1 * (y + 0.5f - v0->y) - 0.5f);
        const int xEnd = (int)ceil(v1->x + invslope2 * (y + 0.5f - v0->y) - 0.5f);

        const float zStart = v0->z + zInvslope1 * (y - v0->y);
        const float zEnd = v1->z + zInvslope2 * (y - v0->y);
        for (int x = xStart; x < xEnd; x++)
        {
            PutPixel(Point{ x, y }, Color, zStart + (zEnd - zStart) * (x - xStart) / (xEnd - xStart));
        }
    }
}

void BufferController::DrawLine(Point a, Point b, Vec3 Color)
{
    int dx = b.x - a.x;
    int dy = b.y - a.y;
    int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);
    float xIncrement = dx / (float)steps;
    float yIncrement = dy / (float)steps;
    float x = a.x;
    float y = a.y;
    for (int i = 0; i <= steps; i++)
    {
        PutPixel(Point{ (int)x, (int)y }, Color);
        x += xIncrement;
        y += yIncrement;
    }
}

void BufferController::PutPixel(Point a, Vec3 Color, float z)
{
    if (a.x < 0 || a.x >= buffer->width || a.y < 0 || a.y >= buffer->height || z > zBuffer[a.x + a.y * buffer->width])
    {
        return;
    }
    zBuffer[a.x + a.y * buffer->width] = z;
    buffer->data[a.x + a.y * buffer->width] = Color;
}
