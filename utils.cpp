#include <string>
#ifdef __cplusplus


struct Vec3 
{
    float x, y, z;
};


struct Buffer 
{
    Vec3* data;
    int size;
};


extern "C" 
{
#endif
    char* finish_string(char* start);
    Buffer* create_buffer(int width, int height);
    void free_buffer(Buffer* buffer);
#ifdef __cplusplus
}


#endif
char* finish_string(char* start)
{
    std::string str = start;
    str += " World";
    return (char*)str.c_str();
}


Buffer* create_buffer(int width, int height)
{
    Buffer* buffer = new Buffer();
    buffer->data = new Vec3[width * height];
    buffer->size = width * height;
    for (int i = 0; i < buffer->size; i++){
        buffer->data[i].x = 1.0f;
        buffer->data[i].y = 1.0f;
        buffer->data[i].z = 1.0f;
    }
    return buffer;
}


void free_buffer(Buffer* buffer)
{
    delete[] buffer->data;
    delete buffer;
}