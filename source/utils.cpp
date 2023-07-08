#include "BufferController.h"

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
#ifdef __cplusplus
}
#endif


