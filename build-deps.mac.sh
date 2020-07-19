#!/bin/bash
set -ex

SOURCE_DIR=${PWD}
CORES=$(sysctl -n hw.ncpu)

COMPILER='-DCMAKE_CXX_COMPILER=/usr/local/bin/g++ -DCMAKE_CC_COMPILER=/usr/local/bin/gcc'

mkdir -p libs

# fetch & build alure
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
    cmake .. $COMPILER -DALSOFT_BACKEND_PULSEAUDIO=OFF && \
    make -j$CORES
    echo "successfully compiled alure"
    cd $SOURCE_DIR
fi


# fetch & build openal-soft
#if [[
#    ! -d "libs/openal-soft/include"
#    # ! -d "libs/openal-soft/include" ||
#    # ! -f "libs/openal-soft/build/libopenal.so"
#]]; then
#    rm -rf "libs/openal-soft"
#    echo "fetching and building openal-soft..."
#    cd libs && \
#    git clone https://github.com/kcat/openal-soft
#    # git clone https://github.com/kcat/openal-soft && \
#    # cd openal-soft && \
#    # git checkout f5e0eef && \
#    # cd build && \
#    # cmake .. && \
#    # make -j$CORES
#    # echo "successfully compiled openal-soft"
#    cd $SOURCE_DIR
#fi


# fetch & build glad
if [[
    ! -d "libs/glad/build/include" ||
    ! -f "libs/glad/build/libglad.a"
]]; then
    rm -rf "libs/glad"
    echo "fetching and building glad..."
    cd libs && \
    git clone https://github.com/Dav1dde/glad.git && \
    cd glad && \
    mkdir build && \
    cd build && \
    cmake .. && \
    make -j$CORES
    echo "successfully compiled glad"
    cd $SOURCE_DIR
fi

# fetch & build lz4
if [[
    ! -f "libs/lz4/lib/liblz4.a"
]]; then
    rm -rf "libs/lz4"
    echo "fetching and building lz4..."
    cd libs && \
    git clone https://github.com/lz4/lz4.git && \
    cd lz4 && \
    make -j$CORES && \
    echo "successfully compiled lz4"
    cd $SOURCE_DIR
fi

# fetch & build NoLifeNx
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
    cmake .. $COMPILER && \
    make -j$CORES && \
    echo "successfully compiled NoLifeNx"
    cd $SOURCE_DIR
fi


# # fetch & build glfw
#if [[
#    ! -d "libs/glfw/include"
#    # ! -d "libs/glfw/include" ||
#    # ! -f "libs/glfw/build/src/libglfw3.a"
#]]; then
#    rm -rf "libs/glfw"
#    echo "fetching and building glfw..."
#    cd libs && \
#    git clone https://github.com/glfw/glfw && \
#    cd glfw && \
#    git checkout 0a49ef0
#    # git checkout 0a49ef0 && \
#    # mkdir build && \
#    # cd build && \
#    # cmake .. && \
#    # make -j$CORES && \
#    # echo "successfully compiled glfw"
#    cd $SOURCE_DIR
#fi


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

# fetch stb
if [[
    ! -d "libs/stb"
]]; then
    rm -rf "libs/stb"
    echo "fetching stb..."
    cd libs && \
    git clone https://github.com/nothings/stb.git && \
    echo "successfully fetched stb"
    cd $SOURCE_DIR
fi
