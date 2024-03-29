import ctypes
from ctypes import *
import pathlib
import numpy as np
import cv2
from math import *
import time
from optimizer import *
from utils import *
from cpp_classes import *
from pynput import mouse, keyboard

funct = "sin(x*3.14/50) * 10 + sin(y*3.14/50) * 10"
@CFUNCTYPE(c_float, c_float, c_float)
def test(x, y):
    global funct
    return eval(funct)

class Engine():
    def __init__(self, config, window_name = 'OptimizerVisualisation'):
        self.window_name = window_name

        self.view_info = ViewInfo()
        self.view_info.deltaPosition.x = 0
        self.view_info.deltaPosition.y = 0
        self.view_info.deltaPosition.z = 0
        self.view_info.rotX = 0
        self.view_info.rotY = 0
        self.view_info.rotZ = 0

        self.view_info.innerRadius = 0.5
        self.view_info.outerRadius = 1.5

        self.trackbarRadiusScale = 33
        
        self.mouse_listener = mouse.Listener(on_move=self.on_move, on_click=self.on_click)
        self.keyboard_listener = keyboard.Listener(on_press=self.on_key_press, on_release=self.on_key_release)

        self.run = True
        self.mouse_pressed_right = False
        self.mouse_pressed_left = False
        self.read_function = False

        self.frame_time = 16

        self.w = config["width"]
        self.h = config["height"]

        libname = pathlib.Path().absolute() /'obj'/'libbuffer.dll'
        self.c_lib = ctypes.cdll.LoadLibrary(str(libname))
       
        for key, value in config.items():
            self.c_lib.AddConfigEntry(key.encode('utf-8'), str(value).encode('utf-8'))

        self.c_lib.BufferController_Create.restype = ctypes.c_void_p
        self.c_lib.GetBuffer.restype = ctypes.c_void_p

        self.buffer_controller = self.c_lib.BufferController_Create(self.w, self.h)

        self.mouse_x = 0
        self.mouse_y = 0

        self.c_lib.InitLandscape(ctypes.c_void_p(self.buffer_controller), test)
        self.optimizer = Optimizer(config)
        self.function = ''

    def on_move(self,x,y):
        if self.mouse_pressed_right:
            self.view_info.rotY += (x-self.mouse_x) * 0.001 * self.frame_time 
            self.view_info.rotX -= (y-self.mouse_y) * 0.001 * self.frame_time
        self.mouse_y=y
        self.mouse_x=x 


    def on_click(self, x, y, button, pressed):
        if button == mouse.Button.left:
            self.mouse_pressed_left = pressed
        elif button == mouse.Button.right:
            self.mouse_pressed_right = pressed
    
    def on_key_press(self, key):
        if key == keyboard.Key.esc:
            self.run = False
        elif self.read_function:
            if key == keyboard.Key.backspace and len(self.function) > 0:
                self.function = self.function[:-1]
            elif key == keyboard.Key.enter:
                self.read_function = False
                if len(self.function) > 0:
                    global funct
                    funct = self.function
                    self.c_lib.InitLandscape(ctypes.c_void_p(self.buffer_controller), test)
            elif hasattr(key, 'char'):
                self.function += key.char
        elif not hasattr(key, 'char'):
            return
        elif key.char == 'w':
            self.view_info.deltaPosition.z = -0.01 * self.frame_time
        elif key.char == 's':
            self.view_info.deltaPosition.z = 0.01 * self.frame_time
        elif key.char == 'a':
            self.view_info.deltaPosition.x = 0.01 * self.frame_time
        elif key.char == 'd':
            self.view_info.deltaPosition.x = -0.01 * self.frame_time
        elif key.char == 'q':
            self.view_info.deltaPosition.y = -0.01 * self.frame_time
        elif key.char == 'e':
            self.view_info.deltaPosition.y = 0.01 * self.frame_time
        elif key.char == '1':
            self.view_info.editMode = NONE
        elif key.char == '2':
            self.view_info.editMode = SCULPT
        elif key.char == '3':
            self.view_info.editMode = MOVE_SPHERE
        elif key.char == '4':
            self.view_info.editMode = RUN
            pos = self.buffer.visualizerPosition   
            self.optimizer.setPos(pos.x,pos.z)
        elif key.char == '5':
            self.read_function = True

    def on_key_release(self, key):
        if hasattr(key, 'char') == False:
            return
        elif key.char == 'w' or key.char == 's':
            self.view_info.deltaPosition.z = 0
        elif key.char == 'a' or key.char == 'd':
            self.view_info.deltaPosition.x = 0
        elif key.char == 'q' or key.char == 'e':
            self.view_info.deltaPosition.y = 0
    
    def change_outer_radius(self, value):
        self.view_info.outerRadius = value/self.trackbarRadiusScale
    
    def change_inner_radius(self, value):
        self.view_info.innerRadius = value/self.trackbarRadiusScale


    def start(self):
        self.mouse_listener.start()
        self.keyboard_listener.start()
        cv2.namedWindow(self.window_name)
        cv2.createTrackbar('innerRadius', self.window_name, int(self.view_info.innerRadius*self.trackbarRadiusScale), 100, self.change_inner_radius)
        cv2.createTrackbar('outerRadius', self.window_name, int(self.view_info.outerRadius*self.trackbarRadiusScale), 100, self.change_outer_radius)     
        while self.run:
            start_frame = current_time()
            
            self.c_lib.FillBuffer(ctypes.c_void_p(self.buffer_controller), ctypes.byref(self.view_info))
            self.buffer = Buffer.from_address(self.c_lib.GetBuffer(ctypes.c_void_p(self.buffer_controller)))  
            data = np.ctypeslib.as_array(self.buffer.data, (self.h,self.w))
            data = data.view((np.float32, 3))
            data = cv2.cvtColor(data, cv2.COLOR_RGB2BGR)
            cv2.putText(data, f'FPS:{format(1000/self.frame_time, ".2f")}',(30,30)
                        ,cv2.FONT_HERSHEY_SIMPLEX,0.3,(0,0,255),1)
            
            if self.read_function:
                cv2.rectangle(data, (0,self.h-30), (self.w, self.h), (0,0,0), -1)
                cv2.putText(data, self.function,(10,self.h-10),cv2.FONT_HERSHEY_SIMPLEX,1,(255,255,255),1)
            
            cv2.imshow(self.window_name, data)

            if self.view_info.editMode == RUN:
                new_x, new_z = self.optimizer.step(self.buffer.gradient.x, self.buffer.gradient.z)
                self.view_info.visualizerDelta.x = new_x - self.buffer.visualizerPosition.x
                self.view_info.visualizerDelta.z = new_z - self.buffer.visualizerPosition.z

            image_transform = cv2.getWindowImageRect(self.window_name)
            self.view_info.mouseX = self.mouse_x - image_transform[0]
            self.view_info.mouseY = self.mouse_y - image_transform[1]
            self.view_info.mouseLeft = self.mouse_pressed_left
            self.view_info.deltaTime = self.frame_time

            cv2.waitKey(1)
            if cv2.getWindowProperty(self.window_name, cv2.WND_PROP_VISIBLE) < 1:
                self.run = False
            self.frame_time =  current_time() - start_frame        
        self.c_lib.BufferController_Destroy(ctypes.c_void_p(self.buffer_controller))
