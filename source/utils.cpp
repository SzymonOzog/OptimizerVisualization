#include "BufferController.h"
#include <iostream>
#include <string>
#ifdef __cplusplus
extern "C" 
{
#endif
    BufferController* BufferController_Create(int width, int height)
    {
        gBufferController = new BufferController(width, height);
        return gBufferController;
    }

    void BufferController_Destroy(BufferController* bufferController)
    {
        if(gBufferController == bufferController)
        {
            gBufferController = nullptr;
        }
        delete bufferController;
    }

    Buffer* GetBuffer(BufferController* bufferController)
    {
        return bufferController->getBuffer();
    }

    void FillBuffer(BufferController* bufferController, const ViewInfo& viewInfo)
    {
        bufferController->fillBuffer(viewInfo);
    }

    void InitLandscape(BufferController* bufferController, float (*callback)(float, float))
    {
        bufferController->initLandscape(callback);
    }

    void AddConfigEntry(const char* key, const char* value)
    {
        gConfig.insert(std::make_pair(std::string(key), std::string(value)));
    }
#ifdef __cplusplus
}
#endif


