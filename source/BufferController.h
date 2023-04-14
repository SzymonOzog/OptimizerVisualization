#include <vector>

struct Mat3
{
    std::vector<std::vector<float>> data;

    Mat3()
    {
        data.resize(3);
        for (int i = 0; i < 3; i++)
        {
            data[i].resize(3);
        }
    }

    static Mat3 Identity()
    {
        Mat3 result;
        result.data[0][0] = 1.0f;
        result.data[1][1] = 1.0f;
        result.data[2][2] = 1.0f;
        return result;
    }

    
};

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