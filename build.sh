./clean.sh

cmake -S . -B CMake -DCMAKE_C_COMPILER=/usr/bin/gcc -DCMAKE_CXX_COMPILER=/usr/bin/g++
cmake --build CMake
mv CMake/Dusk dusk

