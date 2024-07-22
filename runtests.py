import subprocess
from os import environ


BIN_PATH = 'bin/'
SRC_PATH = 'src/'
TESTS_PATH = 'tests/'

subprocess.call(['g++',
                '-fsanitize=address',
                '-O2',
                '-o',
                f'{BIN_PATH}/test',
                '-g',
                f'{TESTS_PATH}/main.cpp',
                # f'{TESTS_PATH}/arr.cpp',
                f'{TESTS_PATH}/hashmap.cpp',
                f'{SRC_PATH}/arr.c',
                f'{SRC_PATH}/hashmap.c',
                '-lgtest',
                ])

environ['ASAN_OPTIONS'] = 'allocator_may_return_null=1'
process = subprocess.run(['./test'], cwd=BIN_PATH)
print(f'Return code is: {process.returncode}')