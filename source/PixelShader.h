#include "DataStructures.h"

class PixelShader
{
public:
    PixelShader(const Vec3& ambientLight, const Vec3& directionalLightColor);
    Vec3 operator () (const Vertex& vertex);
private:
    Vec3 ambientLight = {0.1f, 0.1f, 0.1f};
    Vec3 directionalLightColor = {0.5f, 0.5f, 0.5f};

};