./dusk test.ds
g++ output.o -L. -ldusk -o out
rm output.o
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:./ ./out
