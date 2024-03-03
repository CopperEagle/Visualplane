
# Compile main. Explicitly says where the freetype2 headers are to be found.
g++ -c main.cpp -I/usr/include/freetype2 -I../..

# Do the linking. If you executed the Makefile as stated, then the shared
# library should be in the main directory.
g++ main.o -I. -L. -lstdc++ -Wl,../../libvisualplane.so

# run!
./a.out


