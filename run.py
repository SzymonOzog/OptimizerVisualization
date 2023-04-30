import ctypes
import pathlib
from ctypes.util import find_library
import os
import numpy as np
import cv2
import math

class Vec3(ctypes.Structure):
    _fields_ = [("x", ctypes.c_float),
                ("y", ctypes.c_float),
                ("z", ctypes.c_float)]

class Buffer(ctypes.Structure):
    _fields_ = [("data", ctypes.POINTER(Vec3)),
                ("size", ctypes.c_int)]

class ViewInfo(ctypes.Structure):
    _fields_ = [("rotX", ctypes.c_float),
                ("rotY", ctypes.c_float),
                ("rotZ", ctypes.c_float)]

if __name__ == '__main__':
    if not os.path.exists("obj"):
        os.mkdir("obj")
    os.system('g++ -static -shared -o obj/libbuffer.dll source/*.cpp')

    w,h = 500,500
    libname = pathlib.Path().absolute() /'obj'/'libbuffer.dll'
    c_lib = ctypes.cdll.LoadLibrary(str(libname))

    c_lib.BufferController_Create.restype = ctypes.c_void_p
    c_lib.GetBuffer.restype = ctypes.c_void_p

    buffer_controller = c_lib.BufferController_Create(w,h)
    
    view_info = ViewInfo()
    view_info.rotX = 0
    view_info.rotY = 0
    view_info.rotZ = 0
    
    #while escape key is not pressed
    while cv2.waitKey(16) != 27:
        view_info.rotX = view_info.rotX + 0.01 % 2*math.pi
        view_info.rotY = view_info.rotY + 0.01 % 2*math.pi
        view_info.rotZ = view_info.rotZ + 0.01 % 2*math.pi

        c_lib.FillBuffer(ctypes.c_void_p(buffer_controller), ctypes.byref(view_info))
        buffer = Buffer.from_address(c_lib.GetBuffer(ctypes.c_void_p(buffer_controller)))  

        data = np.ctypeslib.as_array(buffer.data, (w,h))
        data = data.view((np.float32, 3))
        cv2.imshow('test', data)

    c_lib.BufferController_Destroy(ctypes.c_void_p(buffer_controller))