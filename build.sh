./clean.sh

mkdir build
cd build
cmake ..
cmake --build .
cd ..
mv build/Dusk dusk

