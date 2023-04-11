import ctypes
import pathlib
from ctypes.util import find_library
import os
if __name__ == '__main__':
    libname = pathlib.Path().absolute() /'obj'/'libutils.dll'
    print(find_library(str(libname)))
    c_lib = ctypes.cdll.LoadLibrary(str(libname))
    c_lib.finish_string.restype = ctypes.c_char_p

    input = ctypes.c_char_p(b"Hello")
    out = c_lib.finish_string(input)
    print(out)