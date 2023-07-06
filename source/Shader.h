#include "DataStructures.h"

class Shader
{
public:
    Shader(const Vec3& ambientLight, const Vec3& directionalLightColor);
    virtual void initFrame(const ViewInfo& viewInfo, const Mat4& worldViewProjection, const Vec3& actorPosition);
    virtual void transformVertex(Vertex& vertex);
    virtual Vec3 shadePixel(const Vertex& vertex);

private:
    Mat4 worldViewProjection;
    Vec3 actorPosition;
    Vec3 ambientLight;
    Vec3 directionalLightColor;
};