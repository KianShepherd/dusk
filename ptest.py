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
    res = ''
    if file_name == 'io':
        res = subprocess.run(
            ['./out'],
            stdout=subprocess.PIPE,
            input='Hello input()',
            text=True
        ).stdout
    else:
        res = subprocess.run(
            ['./out'],
            stdout=subprocess.PIPE,
            text=True
        ).stdout
    return [x.strip() for x in res.splitlines()]


def run_example(file_name):
    """Run test cases for file."""
    subprocess.run(
        ['./CMake/dusk', f'./examples/{file_name}.ds'],
        stdout=subprocess.DEVNULL,
        stderr=subprocess.DEVNULL,
    )
    try:
        no_op_res = run_output(file_name)
        op_res = run_output(file_name, opt=1)
        expected = run_output(file_name, expected=True)
        
        no_op_diff = []
        no_op_pass = True
        for i in range(max(len(no_op_res), len(expected))):
            if i > len(expected):
                no_op_diff.append(f'""   : "{no_op_res[i]}"')
                no_op_pass = False
            elif i > len(no_op_res):
                no_op_diff.append(f'"{expected[i]}"   :   ""')
                no_op_pass = False
            else:
                if no_op_res[i] != expected[i]:
                    no_op_pass = False
                    no_op_diff.append(f'"{expected[i]}"   :   "{no_op_res[i]}"')

        op_diff = []
        op_pass = True
        for i in range(max(len(op_res), len(expected))):
            if i > len(expected):
                op_diff.append(f'""   : "{op_res[i]}"')
                op_pass = False
            elif i > len(op_res):
                op_diff.append(f'"{expected[i]}"   :   ""')
                op_pass = False
            else:
                if op_res[i] != expected[i]:
                    op_pass = False
                    op_diff.append(f'"{expected[i]}"   :   "{op_res[i]}"')

        preamble = f'{file_name}.ds: '
        while len(preamble) < 20:
            preamble = preamble + " "
        print(f'{preamble}unoptimized {"PASS" if no_op_pass else "FAIL"},'
              f' optimized {"PASS" if op_pass else "FAIL"}')
        if not no_op_pass:
            print("\nUnoptimized Diff\n")
            print('\n'.join(no_op_diff))
        if not op_pass:
            print("\nOptimized Diff\n")
            print('\n'.join(op_diff))
    except FileNotFoundError:
        print(f'Failed to compile {file_name}')


for f in test_cases:
    run_example(f)
