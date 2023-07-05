import os
import sys
from engine import *

if __name__ == '__main__':
    if not os.path.exists("obj"):
        os.mkdir("obj")
    debug_level = '' if sys.argv.count('DEBUG') > 0 else '-O3'
    os.system(f'g++ -static -shared {debug_level} -o obj/libbuffer.dll -ggdb source/*.cpp')
    engine = Engine(1200,800)
    engine.start()