import subprocess


BIN_PATH = 'bin/'
SRC_PATH = 'src/'


subprocess.call(['gcc',
                '-shared',
                '-o',
               f'{BIN_PATH}/libdspdatastructures-0.01v.so',
                '-fPIC',
                '-O2',
                '-Wall',
               f'{SRC_PATH}/arr.c',
               f'{SRC_PATH}/hashmap.c',
                 ])
