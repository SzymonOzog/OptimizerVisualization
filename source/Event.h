#pragma once
#include "DataStructures.h"

class Event
{
public:
    virtual void endEvent() { bIsHandled = true; }
    bool bIsHandled;

};

