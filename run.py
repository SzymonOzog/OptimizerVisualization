import ctypes
import pathlib
from ctypes.util import find_library
import os
import numpy as np
import cv2
import math
import time
from pynput import mouse, keyboard
from cpp_classes import *
from engine import *

if __name__ == '__main__':
    if not os.path.exists("obj"):
        os.mkdir("obj")
    os.system('g++ -static -shared -o obj/libbuffer.dll source/*.cpp')
    engine = Engine(500,500)
    engine.start()