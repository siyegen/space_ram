#!/usr/bin/env bash

echo "building game"

g++ -std=c++11 -W -Wall -Wextra -pedantic -I includes ./*.cpp -o game \
 -lglfw3 -lSOIL -framework Cocoa -framework OpenGL -framework IOKit \
 -framework CoreVideo -lGLEW
