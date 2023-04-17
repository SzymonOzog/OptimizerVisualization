#include "BufferController.h"

#ifdef __cplusplus
extern "C" 
{
#endif
    BufferController* BufferController_Create(int width, int height)
    {
        return new BufferController(width, height);
    }

    void BufferController_Destroy(BufferController* bufferController)
    {
        delete bufferController;
    }

    Buffer* GetBuffer(BufferController* bufferController)
    {
        return bufferController->GetBuffer();
    }

    void FillBuffer(BufferController* bufferController, const ViewInfo& viewInfo)
    {
        bufferController->FillBuffer(viewInfo);
    }
#ifdef __cplusplus
}
#endif


