# Built with arch: amd64 image: ubuntu:18.04
#
################################################################################
# build system
################################################################################
FROM ubuntu:bionic as builder

LABEL maintainer="zeeshan sarwar <dev.786zshan@gmail.com>"

ENV DEBIAN_FRONTEND noninteractive

RUN apt-get update && apt-get install -y \
    apt-transport-https \
    ca-certificates \
    gnupg2 \
    software-properties-common \
    build-essential \
    autoconf \
    libtool \
    pkg-config \
    git \
    wget

RUN wget -qO - https://apt.kitware.com/keys/kitware-archive-latest.asc | apt-key add - && \
  apt-add-repository 'deb https://apt.kitware.com/ubuntu/ bionic main' && \
  apt-get update

RUN apt-get install -y \
  cmake \
  libopenal-dev libvorbis-dev libopusfile-dev libsndfile1-dev \
  xorg-dev \
  libgl1-mesa-dev


# Explicitly copying required files into container (optimize for docker caching)
WORKDIR /tmp/HeavenClient/
COPY *.cpp *.h Icon.* build-deps.sh CMakeLists.txt ./
COPY Audio ./Audio
COPY Character ./Character
COPY Data ./Data
COPY Gameplay ./Gameplay
COPY Graphics ./Graphics
COPY IO ./IO
COPY Net ./Net
COPY Template ./Template
COPY Util ./Util
COPY fonts ./fonts


# Build all HeavenClient dependencies
WORKDIR /tmp/HeavenClient/libs

# fetch & build alure
RUN git clone https://github.com/kcat/alure && \
    cd alure && \
    cd build && \
    cmake .. && \
    make -j$(nproc)

# fetch & build openal-soft
RUN git clone https://github.com/kcat/openal-soft && \
  cd openal-soft && \
  git checkout f5e0eef && \
  cd build && \
  cmake .. && \
  make -j$(nproc)

# fetch & build glad
RUN git clone https://github.com/Dav1dde/glad.git && \
  cd glad && \
  mkdir -p build && \
  cd build && \
  cmake .. && \
  make -j$(nproc)

# fetch & build lz4
RUN git clone https://github.com/lz4/lz4.git && \
  cd lz4 && \
  make -j$(nproc)

# fetch & build NoLifeNx
RUN mkdir NoLifeNx && \
  cd NoLifeNx && \
  git clone https://github.com/lain3d/NoLifeNx nlnx && \
  cd nlnx && \
  mkdir build && \
  cd build && \
  cmake .. && \
  make -j$(nproc)

# fetch & build glfw
RUN git clone https://github.com/glfw/glfw && \
  cd glfw && \
  git checkout 0a49ef0 && \
  mkdir build && \
  cd build && \
  cmake .. && \
  make -j$(nproc)

# fetch asio
RUN git clone https://github.com/chriskohlhoff/asio.git

# fetch stb
RUN git clone https://github.com/nothings/stb.git


# Building HeavenClient
WORKDIR /tmp/HeavenClient
RUN mkdir build && \
  cd build && \
  cmake .. && \
  make -j$(nproc)


# Move HeavenClient into distinct folder
WORKDIR /HeavenClient
RUN mv /tmp/HeavenClient/build/HeavenClient ./ && \
  mv /tmp/HeavenClient/fonts ./ && \
  mv /tmp/HeavenClient/Icon.* ./


################################################################################
# merge
################################################################################
FROM dorowu/ubuntu-desktop-lxde-vnc:bionic

LABEL maintainer="zeeshan sarwar <dev.786zshan@gmail.com>"

ENV DEBIAN_FRONTEND noninteractive

RUN apt-get update && \
  apt-get install -y \
  libfreetype6 \
  libx11-6 \
  libopenal-dev libvorbis-dev libopusfile-dev

# Audio support for Mac (via pulseaudio)
ENV PULSE_SERVER=host.docker.internal
RUN apt-get install -y pulseaudio


# Copy HeavenClient + assets
COPY --from=builder /HeavenClient/ /root/

# Copy libraries
COPY --from=builder /tmp/HeavenClient/libs/alure/build/libalure2.so /usr/lib/x86_64-linux-gnu/libalure2.so

# Set server IP
RUN echo "ServerIP = host.docker.internal" >> /root/Settings

# Create desktop executable for HeavenClient
RUN mkdir -p /root/Desktop/
RUN echo '#!/usr/bin/env xdg-open\n\
[Desktop Entry]\n\
Name=HeavenClient\n\
Comment=Script to run HeavenClient binary\n\
Exec=/root/HeavenClient\n\
Icon=/root/Icon.png\n\
Terminal=false\n\
Type=Application'\
> /root/Desktop/heavenclient.desktop
RUN chmod +x /root/Desktop/heavenclient.desktop

# Set entrypoint for linknx.sh file
COPY ./docker-entrypoint.sh /docker-entrypoint.sh
RUN chmod +x /docker-entrypoint.sh

ENTRYPOINT ["/docker-entrypoint.sh"]
CMD ["/startup.sh"]