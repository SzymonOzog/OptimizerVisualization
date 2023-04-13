import ctypes
import pathlib
from ctypes.util import find_library
import os
import numpy as np
import cv2

class Vec3(ctypes.Structure):
    _fields_ = [("x", ctypes.c_float),
                ("y", ctypes.c_float),
                ("z", ctypes.c_float)]

class Buffer(ctypes.Structure):
    _fields_ = [("data", ctypes.POINTER(Vec3)),
                ("size", ctypes.c_int)]
    

if __name__ == '__main__':

    w,h = 500,500

    libname = pathlib.Path().absolute() /'obj'/'libbuffer.dll'
    c_lib = ctypes.cdll.LoadLibrary(str(libname))

    c_lib.BufferController_Create.restype = ctypes.c_void_p
    c_lib.GetBuffer.restype = ctypes.c_void_p

    buffer_controller = c_lib.BufferController_Create(w,h)
    buffer = Buffer.from_address(c_lib.GetBuffer(ctypes.c_void_p(buffer_controller)))


    data = np.zeros((h,w,3), dtype=np.float32)
    for i in range(h):
        for j in range(w):
            data[i,j] = [buffer.data[i*w+j].x, buffer.data[i*w+j].y, buffer.data[i*w+j].z]
    cv2.imshow('test', data)
    cv2.waitKey(0)

    c_lib.BufferController_Destroy(buffer_controller)