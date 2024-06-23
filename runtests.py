import subprocess


BIN_PATH = 'bin/'
SRC_PATH = 'src/'
TESTS_PATH = 'tests/'

subprocess.call(['gcc',
                 '-o',
                 f'{BIN_PATH}/test',
                 '-g',
                 f'{TESTS_PATH}/main.c',
                 f'{SRC_PATH}/arr.c'])

process = subprocess.run(['./test'], cwd=BIN_PATH)
print(f'Return code is: {process.returncode}')