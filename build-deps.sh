#!/bin/bash

CORES=4

if [ -d "libs/alure" ]; then
    rm -rf "libs/alure"
fi

if [ -d "libs/openal-soft" ]; then
    rm -rf "libs/openal-soft"
fi

if [ -d "libs/glad/build" ]; then
    rm -rf "libs/glad/build"
fi

if [ -d "libs/NoLifeNx" ]; then
    rm -rf "libs/NoLifeNx"
fi

if [ -d "libs/glfw" ]; then
    rm -rf "libs/glfw"
fi

cd libs && \
git clone https://github.com/kcat/alure && cd alure 
cd build && cmake .. && make -j$CORES && cd ../../ && \
\
git clone https://github.com/kcat/openal-soft && cd openal-soft && \
git checkout f5e0eef && cd build && cmake .. && make -j$CORES && cd ../../ && \
\
cd glad && mkdir build && cd build && cmake .. && make -j$CORES && cd ../../ && \
\
cd lz4 && make -j$CORES && cd ../ && \
\
git clone https://github.com/lain3d/NoLifeNx && \
cd NoLifeNx && mkdir build && cd build && cmake .. && make -j$CORES && cd ../../ && \
\
git clone https://github.com/glfw/glfw && cd glfw && git checkout 0a49ef0 && \
mkdir build && cd build && cmake .. && make -j$CORES

