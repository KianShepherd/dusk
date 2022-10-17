./clean.sh

cmake -G Ninja -S . -B CMake
ninja -C CMake

./CMake/dusk test.ds
./out
