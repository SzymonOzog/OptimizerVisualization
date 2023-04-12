#include <string>
#ifdef __cplusplus


struct Vec3 {
    float x, y, z;
};

struct Buffer {
    Vec3* data;
    int size;
};

extern "C" {
#endif
    char* finish_string(char* start);

#ifdef __cplusplus
}

#endif
char* finish_string(char* start){
    std::string str = start;
    str += " World";
    return (char*)str.c_str();
}

