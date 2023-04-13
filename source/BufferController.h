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


class BufferController
{
public:
    BufferController(int width, int height);
    ~BufferController();

    Buffer* GetBuffer();

private:
    void DrawLine(Buffer* buffer, Point a, Point b, Vec3 Color);
    void PutPixel(Buffer* buffer, Point a, Vec3 Color);

    Buffer* buffer;

};