./clean.sh

cmake -G Ninja -S . -B CMake
ninja -C CMake
mv CMake/Dusk dusk

./dusk test.ds
g++ CMake/libdusk.so output.o -o out
rm output.o
./out
echo $?
