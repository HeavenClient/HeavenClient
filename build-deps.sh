#!/bin/bash
set -ex

SOURCE_DIR=${PWD}
CORES=$(nproc)


# build alure
if [[
    ! -d "libs/alure/include" ||
    ! -f "libs/alure/build/libalure2.so" 
]]; then
    rm -rf "libs/alure"
    echo "fetching and building alure..."
    cd libs && \
    git clone https://github.com/kcat/alure && \
    cd alure && \
    cd build && \
    cmake .. && \
    make -j$CORES
    echo "successfully compiled alure"
    cd $SOURCE_DIR
fi


# build openal-soft 
if [[
    ! -d "libs/openal-soft/include" ||
    ! -f "libs/openal-soft/build/libopenal.so"
]]; then
    rm -rf "libs/openal-soft"
    echo "fetching and building openal-soft..."
    cd libs && \
    git clone https://github.com/kcat/openal-soft && \
    cd openal-soft && \
    git checkout f5e0eef && \
    cd build && \
    cmake .. && \
    make -j$CORES
    echo "successfully compiled openal-soft"
    cd $SOURCE_DIR
fi


# build glad
if [[
    ! -d "libs/glad/build/include" ||
    ! -f "libs/glad/build/libglad.a"
]]; then
    rm -rf "libs/glad/build"
    echo "building glad..."
    cd libs && \
    cd glad && \
    mkdir build && \
    cd build && \
    cmake .. && \
    make -j$CORES
    echo "successfully compiled glad"
    cd $SOURCE_DIR
fi


# build lz4
if [[
    ! -f "libs/lz4/lib/liblz4.a"
]]; then
    echo "building lz4..."
    cd libs/lz4 && \
    make -j$CORES && \
    echo "successfully compiled lz4"
    cd $SOURCE_DIR
fi

# build NoLifeNx
if [[
    ! -f "libs/NoLifeNx/nlnx/build/libNoLifeNx.a"
]]; then
    rm -rf "libs/NoLifeNx"
    echo "fetching and building NoLifeNx..."
    cd libs && \
    mkdir NoLifeNx && \
    cd NoLifeNx && \
    git clone https://github.com/lain3d/NoLifeNx nlnx && \
    cd nlnx && \
    mkdir build && \
    cd build && \
    cmake .. && \
    make -j$CORES && \
    echo "successfully compiled NoLifeNx"
    cd $SOURCE_DIR
fi


# build glfw
if [[
    ! -d "libs/glfw/include" ||
    ! -f "libs/glfw/build/src/libglfw3.a"
]]; then
    rm -rf "libs/glfw"
    echo "fetching and building glfw..."
    cd libs && \
    git clone https://github.com/glfw/glfw && \
    cd glfw && \
    git checkout 0a49ef0 && \
    mkdir build && \
    cd build && \
    cmake .. && \
    make -j$CORES && \
    echo "successfully compiled glfw"
    cd $SOURCE_DIR
fi


# fetch asio
if [[
    ! -d "libs/asio/asio/include"
]]; then
    rm -rf "libs/asio"
    echo "fetching asio..."
    cd libs && \
    git clone https://github.com/chriskohlhoff/asio.git && \
    echo "successfully fetched asio"
    cd $SOURCE_DIR
fi

