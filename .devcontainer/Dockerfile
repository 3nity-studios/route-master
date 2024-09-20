FROM mcr.microsoft.com/devcontainers/cpp:1-debian-11

ARG REINSTALL_CMAKE_VERSION_FROM_SOURCE="3.30.2"

COPY ./reinstall-cmake.sh /tmp/

RUN if [ "${REINSTALL_CMAKE_VERSION_FROM_SOURCE}" != "none" ]; then \
        chmod +x /tmp/reinstall-cmake.sh && /tmp/reinstall-cmake.sh ${REINSTALL_CMAKE_VERSION_FROM_SOURCE}; \
    fi \
    && rm -f /tmp/reinstall-cmake.sh

RUN apt-get -y -qq update && export DEBIAN_FRONTEND=noninteractive \
    && apt-get -y install --no-install-recommends \
    libdrm-amdgpu1 \
    libdrm-common \
    libdrm-intel1 \
    libdrm-nouveau2 \
    libdrm-radeon1 \
    libdrm2 \
    libegl-dev \
    libegl-mesa0 \
    libegl1 \
    libgbm1 \
    libgl-dev \
    libgl1 \
    libgl1-mesa-dri \
    libglapi-mesa \
    libgles-dev \
    libgles1 \
    libgles2 \
    libglvnd-dev \
    libglvnd0 \
    libglx-dev \
    libglx-mesa0 \
    libglx0 \
    libopengl-dev \
    libopengl0 \
    libpciaccess0 \
    libpthread-stubs0-dev \
    libsensors-config \
    libsensors5 \
    libvulkan1 \
    libwayland-client0 \
    libwayland-server0 \
    libx11-xcb1 \
    libxau-dev \
    libxcb-dri2-0 \
    libxcb-dri3-0 \
    libxcb-glx0 \
    libxcb-present0 \
    libxcb-randr0 \
    libxcb-shm0 \
    libxcb-sync1 \
    libxcb-xfixes0 \
    libxcb1-dev \
    libxdamage1 \
    libxdmcp-dev \
    libxshmfence1 \
    libxxf86vm1 \
    mesa-vulkan-drivers \
    x11proto-dev \
    xorg-sgml-doctools \
    xtrans-dev \
    libasound2-dev \
    libx11-dev \
    libxrandr-dev \
    libxi-dev \
    libgl1-mesa-dev \
    libglu1-mesa-dev \
    libxcursor-dev \
    libxinerama-dev \
    libwayland-dev \
    libxkbcommon-dev \
    libudev-dev \
    libfreetype-dev \
    libopenal-dev \
    libflac-dev \
    libvorbis-dev \
    g++ \
    clang \
    libc++-dev \
    libc++abi-dev \
    ninja-build \
    libfontconfig1-dev \
    build-essential \
    libtool \
    autoconf \
    unzip \
    wget \
    clang-format \
    clang-tidy \
    python3-pip \
    && python3 -m pip install codespell
    && rm -rf /var/lib/apt/lists/*

RUN git clone https://github.com/SFML/SFML.git --branch 3.0.0-rc.1 --depth 1 \
    && cd SFML \
    && cmake -S . -B build && cmake --build build && cmake --install build \
    && cd .. && rm -rf SFML \
    && git clone https://github.com/3nity-studios/tmxlite.git --branch v1.4.4.1 --depth 1 \
    && cd tmxlite \
    && cmake -S tmxlite -B build && cmake --build build && cmake --install build \
    && cd .. && rm -rf tmxlite \
    && git clone https://github.com/3nity-studios/DeSiGNAR.git --branch v2.0.1 --depth 1 \
    && cd DeSiGNAR \
    && cmake -S . -B build && cmake --build build && cmake --install build \
    && cd .. && rm -rf DeSiGNAR \
    && git clone https://github.com/catchorg/Catch2.git --branch v3.7.0 --depth 1 \
    && cd Catch2 \
    && cmake -S . -B build \
    && cmake --build build && cmake --install build \
    && cd .. && rm -rf Catch2 \
    && git clone https://github.com/nlohmann/json.git --branch v3.11.3 --depth 1 \
    && cd json \
    && cmake -S . -B build -DJSON_BuildTests=OFF \
    && cmake --build build && cmake --install build \
    && cd .. && rm -rf json
