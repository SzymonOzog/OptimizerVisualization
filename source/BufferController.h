#pragma once
#include "DataStructures.h"
#include <memory>
#include <unordered_map>
#include <string>

class Actor;
class Event;
class GetVisualiserGradientEvent;
class GetVisualiserPositionEvent;
class Shader;

class BufferController
{
public:
    BufferController(int width, int height);
    ~BufferController();

    void fillBuffer(const ViewInfo& viewInfo);
    Buffer* getBuffer();

    EditMode getEditMode();

    void addEvent(std::shared_ptr<Event> e);

    void initLandscape(float (*callback)(float, float));
private:
    void clearBuffer();

    void dispatchEvents(const std::unique_ptr<Actor>& actor);

    void drawTriangle(Vertex* v0, Vertex* v1, Vertex* v2, std::shared_ptr<Shader> ps);
    void drawFlatTriangle(Vertex* v0, Vertex* v1, Vertex* v2, std::shared_ptr<Shader> ps);

    void drawLine(Point a, Point b, Vec3 Color);
    void putPixel(Point a, Vec3 Color, float z = 0.0f);

    Vec3 getColor(const Vec3& vertex, const Vec3& normal, Vec3 unlitColor);
    Buffer* buffer;
    std::vector<float> zBuffer;
    std::vector<std::unique_ptr<Actor>> actors;
    std::vector<std::shared_ptr<Event>> events;

    std::shared_ptr<GetVisualiserGradientEvent> visualiserGradientEvent;
    std::shared_ptr<GetVisualiserPositionEvent> visualiserPositionEvent;

    float nearPlane;
    float farPlane;
    float fov;
    Mat4 projectionMatrix;

    Mat4 cameraRotationInverse;
    Vec3 cameraPosition;

    EditMode editMode;

};

extern BufferController* gBufferController;
extern std::unordered_map<std::string, std::string> gConfig;
