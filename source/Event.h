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