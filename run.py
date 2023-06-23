import os
from engine import *

if __name__ == '__main__':
    if not os.path.exists("obj"):
        os.mkdir("obj")
    os.system('g++ -static -shared -O3 -o obj/libbuffer.dll source/*.cpp')
    engine = Engine(1200,800)
    engine.start()