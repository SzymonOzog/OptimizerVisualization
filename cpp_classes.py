import ctypes

class Vec3(ctypes.Structure):
    _fields_ = [("x", ctypes.c_float),
                ("y", ctypes.c_float),
                ("z", ctypes.c_float)]

class Buffer(ctypes.Structure):
    _fields_ = [("data", ctypes.POINTER(Vec3)),
                ("width", ctypes.c_int),
                ("height", ctypes.c_int),
                ("gradient", Vec3),
                ("visualizerPosition", Vec3)]

class ViewInfo(ctypes.Structure):
    _fields_ = [("rotX", ctypes.c_float),
                ("rotY", ctypes.c_float),
                ("rotZ", ctypes.c_float),
                ("deltaPosition", Vec3),
                ("mouseX", ctypes.c_int),
                ("mouseY", ctypes.c_int),
                ("mouseLeft", ctypes.c_bool),
                ("deltaTime", ctypes.c_int),
                ("innerRadius", ctypes.c_float),
                ("outerRadius", ctypes.c_float),
                ("editMode", ctypes.c_uint8),
                ("visualizerDelta", Vec3)]
    
(NONE, SCULPT, MOVE_SPHERE, RUN) = (0, 1, 2, 3)