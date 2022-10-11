./clean.sh

cmake -G Ninja -S . -B CMake
ninja -C CMake
mv CMake/Dusk dusk
mv CMake/libdusk.so libdusk.so

./dusk test.ds
g++ output.o -L. -ldusk -o out
rm output.o
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:./ ./out
