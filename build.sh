./clean.sh
flex src/tokens.l
bison src/grammar.y

mkdir build
cd build
cmake ..
cmake --build .
cd ..
mv build/Dusk dusk

