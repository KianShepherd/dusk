import os
import subprocess


os.system('./clean.sh')
os.system('./build.sh')


def run_example(file_name, optimized):
    os.system('./CMake/dusk {"-O" if optimized else ""} ./examples/{file_name}')
    res = subprocess.run(['./out'], stdout=subprocess.PIPE, text=True)
