./clean.sh

cmake -S . -B CMake
cmake --build CMake
mv CMake/Dusk dusk

