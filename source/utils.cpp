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
#ifdef __cplusplus
}
#endif


