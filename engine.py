import ctypes
import pathlib
import numpy as np
import cv2
import math
import time
from utils import *
from cpp_classes import *
from pynput import mouse, keyboard

class Engine():
    def __init__(self, w, h):
        self.view_info = ViewInfo()
        self.view_info.rotX = 0
        self.view_info.rotY = 0
        self.view_info.rotZ = 0
        self.view_info.position.x = 0
        self.view_info.position.y = 0
        self.view_info.position.z = 10
        
        self.mouse_listener = mouse.Listener(on_move=self.on_move, on_click=self.on_click)
        self.keyboard_listener = keyboard.Listener(on_press=self.on_key_press)

        self.run = True
        self.mouse_pressed = False

        self.frame_time = 16

        self.w = w
        self.h = h

        libname = pathlib.Path().absolute() /'obj'/'libbuffer.dll'
        self.c_lib = ctypes.cdll.LoadLibrary(str(libname))

        self.c_lib.BufferController_Create.restype = ctypes.c_void_p
        self.c_lib.GetBuffer.restype = ctypes.c_void_p

        self.buffer_controller = self.c_lib.BufferController_Create(w,h)
        self.mouse_x = 0
        self.mouse_y = 0

    def on_move(self,x,y):
        if self.mouse_pressed:
            self.view_info.rotY = (self.view_info.rotY + ((x-self.mouse_x) * 0.001 * self.frame_time)) % (2 * math.pi)  
            self.view_info.rotX = (self.view_info.rotX + ((y-self.mouse_y) * 0.001 * self.frame_time)) % (2 * math.pi)
        self.mouse_y=y
        self.mouse_x=x 


    def on_click(self, x, y, button, pressed):
        self.mouse_pressed = pressed
    
    def on_key_press(self, key):
            if key == keyboard.Key.esc:
                self.run = False
            elif key.char == 'w':
                self.view_info.position.z += 0.01 * self.frame_time
            elif key.char == 's':
                self.view_info.position.z -= 0.01 * self.frame_time
            elif key.char == 'a':
                self.view_info.position.x += 0.01 * self.frame_time
            elif key.char == 'd':
                self.view_info.position.x -= 0.01 * self.frame_time
    
    def start(self):
        self.mouse_listener.start()
        self.keyboard_listener.start()
        while self.run:
            start_frame = current_time()
            
            self.c_lib.FillBuffer(ctypes.c_void_p(self.buffer_controller), ctypes.byref(self.view_info))
            buffer = Buffer.from_address(self.c_lib.GetBuffer(ctypes.c_void_p(self.buffer_controller)))  

            data = np.ctypeslib.as_array(buffer.data, (self.w,self.h))
            data = data.view((np.float32, 3))

            cv2.putText(data, f'FPS:{format(1000/self.frame_time, ".2f")}',(30,30)
                        ,cv2.FONT_HERSHEY_SIMPLEX,0.3,(0,0,255),1)
            cv2.imshow('test', data)            
            
            image_transform = cv2.getWindowImageRect('test')
            self.view_info.mouse_x = self.mouse_x - image_transform[0]
            self.view_info.mouse_y = self.mouse_y - image_transform[1]
            
            cv2.waitKey(1)

            self.frame_time =  current_time() - start_frame
            if self.frame_time < 16:
                self.frame_time = 16
                time.sleep((16 - self.frame_time) / 1000)

        
        self.c_lib.BufferController_Destroy(ctypes.c_void_p(self.buffer_controller))
        
