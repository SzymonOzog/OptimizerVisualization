import ctypes

class Vec3(ctypes.Structure):
    _fields_ = [("x", ctypes.c_float),
                ("y", ctypes.c_float),
                ("z", ctypes.c_float)]

class Buffer(ctypes.Structure):
    _fields_ = [("data", ctypes.POINTER(Vec3)),
                ("size", ctypes.c_int)]

class ViewInfo(ctypes.Structure):
    _fields_ = [("deltaRotX", ctypes.c_float),
                ("deltaRotY", ctypes.c_float),
                ("deltaRotZ", ctypes.c_float),
                ("deltaPosition", Vec3),
                ("mouseX", ctypes.c_int),
                ("mouseY", ctypes.c_int),
                ("mouseLeft", ctypes.c_bool),
                ("deltaTime", ctypes.c_int),
                ("innerRadius", ctypes.c_float),
                ("outerRadius", ctypes.c_float)]