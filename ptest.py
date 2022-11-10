#!/usr/bin/env python
"""Test suit for dusk functionality.

Runs examples found in ./examples/ against known good
output files found in ./examples/example_outputs/
"""
import os
import subprocess
import time
from os.path import isfile, join

example_path = './examples/'

test_cases = [
    str(f).split('.')[0]
    for f in os.listdir(example_path)
    if isfile(join(example_path, f))
]

print('Cleaning old files.')
subprocess.call(
    ['sh', './clean.sh'],
    stdout=subprocess.DEVNULL,
    stderr=subprocess.DEVNULL,
)
print('Done.')
print('Building dusk.')
s = time.time()
subprocess.call(
    ['sh', './build.sh'],
    stdout=subprocess.DEVNULL,
    stderr=subprocess.DEVNULL,
)
e = time.time()
print(f'Done, took {e - s} seconds.')


def run_output(file_name, *, opt=None, expected=False):
    """Run test case."""
    if expected:
        with open(f'./examples/example_outputs/{file_name}.txt') as f:
            return f.read()

    command = ['./CMake/dusk']
    if opt is not None:
        command.append('-O')
    command.append(f'./examples/{file_name}.ds')
    subprocess.run(
        command,
        stdout=subprocess.DEVNULL,
        stderr=subprocess.DEVNULL,
    )
    if file_name == 'io':
        return subprocess.run(
            ['./out'],
            stdout=subprocess.PIPE,
            input='Hello input()',
            text=True
        ).stdout
    else:
        return subprocess.run(
            ['./out'],
            stdout=subprocess.PIPE,
            text=True
        ).stdout


def run_example(file_name):
    """Run test cases for file."""
    subprocess.run(
        ['./CMake/dusk', f'./examples/{file_name}.ds'],
        stdout=subprocess.DEVNULL,
        stderr=subprocess.DEVNULL,
    )
    no_op_res = run_output(file_name)
    op_res = run_output(file_name, opt=1)
    expected = run_output(file_name, expected=True)

    no_op_pass = no_op_res == expected
    op_pass = op_res == expected
    preamble = f'{file_name}.ds: '
    while len(preamble) < 20:
        preamble = preamble + " "
    print(f'{preamble}unoptimized {"PASS" if no_op_pass else "FAIL"},'
          f' optimized {"PASS" if no_op_pass else "FAIL"}')
    if not (no_op_pass and op_pass):
        print(op_res)
        print(no_op_res)
        print(expected)


for f in test_cases:
    run_example(f)
