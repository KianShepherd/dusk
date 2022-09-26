./clean.sh

cmake -S . -B build
cmake --build build
mv build/Dusk dusk

