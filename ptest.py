import os
import subprocess

from os.path import isfile, join

example_path = './examples/'

test_cases = [str(f).split('.')[0] for f in os.listdir(example_path) if isfile(join(example_path, f))]

os.system('./clean.sh')
os.system('./build.sh')


def run_example(file_name):
    subprocess.run([f'./CMake/dusk', f'./examples/{file_name}.ds'],
        stdout=subprocess.DEVNULL,
        stderr=subprocess.DEVNULL,
    )
    no_op_res = subprocess.run(['./out'], stdout=subprocess.PIPE, text=True).stdout
    subprocess.run([f'./CMake/dusk', '-O', f'./examples/{file_name}.ds'],
        stdout=subprocess.DEVNULL,
        stderr=subprocess.DEVNULL,
    )
    op_res = subprocess.run(['./out'], stdout=subprocess.PIPE, text=True).stdout

    expected = ''
    with open(f'./example_outputs/{file_name}.txt') as f:
        expected = f.read()

    print(f'Unoptimized {file_name}.ds: ', end='')
    if no_op_res != expected:
        print('FAIL')
    else:
        print('PASS')
    print(f'Optimized {file_name}.ds: ', end='')
    if op_res != expected:
        print('FAIL')
    else:
        print('PASS')


for f in test_cases:
    run_example(f)
