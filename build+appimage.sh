#!/bin/sh

echo "Building..."

export PATH=${HOME}/dev/x86_64/sdl2/bin:${PATH}

make clean
make CPPFLAGS="-std=c++11 -I${HOME}/dev/x86_64/sdl2/include -I${HOME}/dev/x86_64/sdl2/include/SDL2" LDFLAGS="-L${HOME}/dev/x86_64/sdl2/lib -L/usr/lib"

echo "Making AppImage..."

./appimage-x86_64.sh 
