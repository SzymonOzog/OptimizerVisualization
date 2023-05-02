import ctypes
import pathlib
from ctypes.util import find_library
import os
import numpy as np
import cv2
import math
import time
from pynput import mouse, keyboard


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
run = True
def on_move(x,y):
    global mouse_pressed
    global mouse_x
    global mouse_y
    if mouse_pressed:
        view_info.rotY = (view_info.rotY + ((x-mouse_x) * 0.001 * frame_time)) % (2 * math.pi)  
        view_info.rotX = (view_info.rotX + ((y-mouse_y) * 0.001 * frame_time)) % (2 * math.pi)
        mouse_y=y
        mouse_x=x 

def on_click(x,y,button, pressed):
    global mouse_pressed
    global mouse_x
    global mouse_y
    mouse_pressed = pressed
    mouse_x = x
    mouse_y = y

def on_key_press(key):
        print(key)
        if key == keyboard.Key.esc:
            global run
            run = False
        elif key.char == 'w':
            view_info.position.z += 0.01 * frame_time
        elif key.char == 's':
            view_info.position.z -= 0.01 * frame_time
        elif key.char == 'a':
            view_info.position.x += 0.01 * frame_time
        elif key.char == 'd':
            view_info.position.x -= 0.01 * frame_time

if __name__ == '__main__':
    if not os.path.exists("obj"):
        os.mkdir("obj")
    os.system('g++ -static -shared -o obj/libbuffer.dll source/*.cpp')


    mouse_listener = mouse.Listener(on_move=on_move, on_click=on_click)
    mouse_listener.start()

    keyboard_listener = keyboard.Listener(on_press=on_key_press)
    keyboard_listener.start()

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
    while run:
        start_frame = current_time()
        
        c_lib.FillBuffer(ctypes.c_void_p(buffer_controller), ctypes.byref(view_info))
        buffer = Buffer.from_address(c_lib.GetBuffer(ctypes.c_void_p(buffer_controller)))  

        data = np.ctypeslib.as_array(buffer.data, (w,h))
        data = data.view((np.float32, 3))

        cv2.putText(data, f'FPS:{format(1000/frame_time, ".2f")}',(30,30)
                    ,cv2.FONT_HERSHEY_SIMPLEX,0.3,(0,0,255),1)
        cv2.imshow('test', data)
        key = cv2.waitKey(1)

        frame_time =  current_time() - start_frame
        if frame_time < 16:
            frame_time = 16
            time.sleep((16 - frame_time) / 1000)

    
    c_lib.BufferController_Destroy(ctypes.c_void_p(buffer_controller))