import os
from engine import *

if __name__ == '__main__':
    if not os.path.exists("obj"):
        os.mkdir("obj")
    os.system('g++ -static -shared -O3 -o obj/libbuffer.dll source/*.cpp')
    engine = Engine(800,800)
    engine.start()