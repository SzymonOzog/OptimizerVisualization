import ctypes
import pathlib
from ctypes.util import find_library
import os
import numpy as np
import cv2
import math
import time
from pynput import mouse


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
                ("rotZ", ctypes.c_float),
                ("position", Vec3)]
    
def current_time():
    return time.time_ns() // 1_000_000

mouse_pressed = False
mouse_x=-1
mouse_y=-1
def on_move(x,y):
    global mouse_pressed
    global mouse_x
    global mouse_y
    if mouse_pressed:
        print(x, mouse_x)
        view_info.rotY = (view_info.rotY + ((x-mouse_x) * 0.001 * frame_time)) % (2 * math.pi)  
        view_info.rotX = (view_info.rotX + ((y-mouse_y) * 0.001 * frame_time)) % (2 * math.pi)
        mouse_y=y
        mouse_x=x 

def on_click(x,y,button, pressed):
    global mouse_pressed
    global mouse_x
    global mouse_y
    mouse_pressed = pressed
    print(pressed, mouse_pressed)
    mouse_x = x
    mouse_y = y

if __name__ == '__main__':
    if not os.path.exists("obj"):
        os.mkdir("obj")
    os.system('g++ -static -shared -o obj/libbuffer.dll source/*.cpp')
    listener = mouse.Listener(on_move=on_move, on_click=on_click)
    listener.start()
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
    view_info.position.x = 0
    view_info.position.y = 0
    view_info.position.z = 10
    
    frame_time = 16
    #while escape key is not pressed
    while True:
        start_frame = current_time()
        #view_info.rotX = (view_info.rotX + (0.001 * frame_time)) % (2 * math.pi)  
        #view_info.rotY = (view_info.rotY + (0.001 * frame_time)) % (2 * math.pi) 
        #view_info.rotZ = (view_info.rotZ + (0.001 * frame_time)) % (2 * math.pi) 
        
        c_lib.FillBuffer(ctypes.c_void_p(buffer_controller), ctypes.byref(view_info))
        buffer = Buffer.from_address(c_lib.GetBuffer(ctypes.c_void_p(buffer_controller)))  

        data = np.ctypeslib.as_array(buffer.data, (w,h))
        data = data.view((np.float32, 3))
        cv2.imshow('test', data)
        key = cv2.waitKey(1)
        if key == 27:
            break
        elif key == ord('w'):
            view_info.position.z += 0.01 * frame_time
        elif key == ord('s'):
            view_info.position.z -= 0.01 * frame_time
        elif key == ord('a'):
            view_info.position.x += 0.01 * frame_time
        elif key == ord('d'):
            view_info.position.x -= 0.01 * frame_time
        
        frame_time =  current_time() - start_frame
        if frame_time < 16:
            time.sleep((16 - frame_time) / 1000)

    
    c_lib.BufferController_Destroy(ctypes.c_void_p(buffer_controller))