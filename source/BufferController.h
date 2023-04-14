#include <vector>

struct Vec3 
{
    float x, y, z;
};


struct Buffer 
{
    Vec3* data;
    int width, height;
};

struct Point
{
    int x, y;
};


struct IndexedLineVector
{
    std::vector<Vec3> vertices;
    std::vector<Point> projectedVertices;
    std::vector<int> indices;
};


class BufferController
{
public:
    BufferController(int width, int height);
    ~BufferController();

    Buffer* GetBuffer();

  

private:

    Point ProjectToScreen(const Vec3& vertex);
    void DrawLine(Buffer* buffer, Point a, Point b, Vec3 Color);
    void PutPixel(Buffer* buffer, Point a, Vec3 Color);

    Buffer* buffer;

};