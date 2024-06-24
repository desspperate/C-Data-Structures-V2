import subprocess


BIN_PATH = 'bin/'
SRC_PATH = 'src/'
TESTS_PATH = 'tests/'

subprocess.call(['g++',
                 '-o',
                 f'{BIN_PATH}/test',
                 '-g',
                 f'{TESTS_PATH}/main.cpp',
                 f'{TESTS_PATH}/arr.cpp',
                 f'{SRC_PATH}/arr.c',
                '-lgtest',
                '-lpthread'
                ])

process = subprocess.run(['./test'], cwd=BIN_PATH)
print(f'Return code is: {process.returncode}')