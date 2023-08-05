#pragma once
#include "DataStructures.h"

class Event
{
public:
    Event() : bIsHandled(false) {}
    virtual void endEvent() { bIsHandled = true; }
    bool bIsHandled;

};

class GetMousePositionWorldSpaceEvent : public Event
{ 
public:
    Vec3 mousePosition;
};

class SetVisualiserPositionEvent : public Event
{
public:
    SetVisualiserPositionEvent(const Vec3& position) : position(position) {}
    Vec3 position;
};

class GetVisualiserGradientEvent : public Event
{
public:
    Vec3 gradient;
};

class MoveVisualiserEvent : public Event
{
public:
    MoveVisualiserEvent(const Vec3& delta) : delta(delta) {}
    Vec3 delta;
};

class GetVisualiserPositionEvent : public Event
{
public:
    Vec3 position;
};

class InitLandscapeEvent : public Event
{
public:
    InitLandscapeEvent(float (*callback)(float, float)) : callback(callback) {}
    float (*callback)(float, float);
};