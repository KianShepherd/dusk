./dusk test.ds
g++ output.o -L. -ldusk -o out
rm output.o
./out
