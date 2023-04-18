#include <vector>
#include <cmath>


struct Vec3 
{
    float x, y, z;
};

struct Mat3
{
    std::vector<std::vector<float>> data;

    Mat3()
    {
        data.resize(3);
        for (int i = 0; i < 3; i++)
        {
            data[i].resize(3);
            data[i] = { 0.0f, 0.0f, 0.0f };
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

    static Mat3 RotationZ(float angle)
    {
        Mat3 result;
        result.data[0][0] = cos(angle);
        result.data[0][1] = -sin(angle);
        result.data[1][0] = sin(angle);
        result.data[1][1] = cos(angle);
        result.data[2][2] = 1.0f;
        return result;
    }

    static Mat3 RotationY(float angle)
    {
        Mat3 result;
        result.data[0][0] = cos(angle);
        result.data[0][2] = sin(angle);
        result.data[1][1] = 1.0f;
        result.data[2][0] = -sin(angle);
        result.data[2][2] = cos(angle);
        return result;
    }

    static Mat3 RotationX(float angle)
    {
        Mat3 result;
        result.data[0][0] = 1.0f;
        result.data[1][1] = cos(angle);
        result.data[1][2] = -sin(angle);
        result.data[2][1] = sin(angle);
        result.data[2][2] = cos(angle);
        return result;
    }


    Vec3 operator*(const Vec3& other)
    {
        Vec3 result;
        result.x = data[0][0] * other.x + data[0][1] * other.y + data[0][2] * other.z;
        result.y = data[1][0] * other.x + data[1][1] * other.y + data[1][2] * other.z;
        result.z = data[2][0] * other.x + data[2][1] * other.y + data[2][2] * other.z;
        return result;
    }

    Mat3 operator*(const Mat3& other)
    {
        Mat3 result;
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                result.data[i][j] = data[i][0] * other.data[0][j] + data[i][1] * other.data[1][j] + data[i][2] * other.data[2][j];
            }
        }
        return result;
    }
};

struct ViewInfo
{
    float RotX;
    float RotY;
    float RotZ;
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

    void FillBuffer(const ViewInfo& viewInfo);
    Buffer* GetBuffer();

private:

    Point ProjectToScreen(const Vec3& vertex);
    void DrawLine(Point a, Point b, Vec3 Color);
    void PutPixel(Point a, Vec3 Color);

    Buffer* buffer;

};