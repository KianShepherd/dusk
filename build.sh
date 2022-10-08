./clean.sh

cmake -G Ninja -S . -B CMake -DCMAKE_C_COMPILER=/usr/bin/gcc -DCMAKE_CXX_COMPILER=/usr/bin/g++
ninja -C CMake
mv CMake/Dusk dusk

