./clean.sh

cmake -G Ninja -S . -B CMake
ninja -C CMake
mv CMake/Dusk dusk
mv CMake/libdusk.a libdusk.a
