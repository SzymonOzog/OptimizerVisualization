#include <string>
#ifdef __cplusplus
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

