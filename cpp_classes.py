import ctypes

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
                ("position", Vec3),
                ("mouseX", ctypes.c_int),
                ("mouseY", ctypes.c_int),
                ("mouseLeft", ctypes.c_bool),
                ("deltaTime", ctypes.c_int),
                ("innerRadius", ctypes.c_float),
                ("outerRadius", ctypes.c_float)]