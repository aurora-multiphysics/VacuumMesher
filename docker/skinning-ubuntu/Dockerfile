# Get base image
FROM idaholab/moose:latest

# By default one core is used to compile
ARG compile_cores=6

# By default checkout mfem-moose branch
ARG WORKDIR="opt"
ARG build_git_sha="master"

# Install apt dependencies
RUN apt-get update && DEBIAN_FRONTEND=noninteractive TZ=Etc/UTC apt-get install -y \
    clang-format-9 \
    curl \
    libboost-all-dev \
    libnetcdf-dev \
    libhdf5-mpich-dev \
    libssl-dev \
    xxd \
    libfontconfig1-dev \
    libfreetype6-dev \
    libsdl2-dev \
    libglew-dev \
    libglm-dev \
    libpng-dev \
    ninja-build

# Install CMake
RUN cd /$WORKDIR && \
    git clone https://github.com/Kitware/CMake.git && \
    cd CMake/ && \
    git checkout 78ae759 && \
    ./bootstrap --parallel="$compile_cores" && \
    make -j"$compile_cores" && \
    make install && \
    alias cmake='/$WORKDIR/CMake/bin/cmake'

# Get libIGL

RUN cd /$WORKDIR && \
    git clone https://github.com/libigl/libigl.git && \
    cd libigl && \
    git checkout main

# Get libmesh-skinnin

RUN cd /$WORKDIR && \
    git clone https://github.com/aurora-multiphysics/libmeshSkinning.git && \
    cd libmeshSkinning && \
    git checkout manifoldSplitter && \
    mkdir build && \
    cd build && \
    cmake -DFETCHCONTENT_SOURCE_DIR_LIBIGL=/opt/libigl -DMOOSE_DIR=/opt/moose ../





    