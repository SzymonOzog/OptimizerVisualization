import os
import sys
import json
from engine import *

if __name__ == '__main__':
    if not os.path.exists("obj"):
        os.mkdir("obj")
    debug_level = '' if sys.argv.count('DEBUG') > 0 else '-O3'
    config_path = 'config.json' if sys.argv.count('config') == 0 else sys.argv[sys.argv.index('config')+1]
    config = json.load(open(config_path))
    os.system(f'g++ -static -shared {debug_level} -o obj/libbuffer.dll -ggdb source/*.cpp')
    engine = Engine(config)
    engine.start()