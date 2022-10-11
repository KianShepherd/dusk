sudo ./clean.sh

cmake -G Ninja -S . -B CMake
ninja -C CMake
sudo ninja -C CMake install
