# Get base image
FROM idaholab/moose:latest

# By default one core is used to compile
ARG compile_cores=2

ARG WORKDIR="opt"
ARG BUILD_GIT_SHA
RUN echo ${BUILD_GIT_SHA}
RUN apt update -y && apt upgrade -y
# Install apt dependencies
RUN apt update && DEBIAN_FRONTEND=noninteractive TZ=Etc/UTC apt install -y \
    curl \
    libboost-all-dev \
    # openmpi-bin \
    libnetcdf-dev \
    libhdf5-openmpi-dev \
    libssl-dev \
    xxd \
    libfontconfig1-dev \
    libfreetype6-dev \
    libsdl2-dev \
    libglew-dev \
    libglm-dev \
    libpng-dev \
    ninja-build \
    build-essential \
    git 
    # python3 \
    # python-is-python3 \
    # flex bison

# Install CMake
RUN cd /$WORKDIR && \
    git clone https://github.com/Kitware/CMake.git && \
    cd CMake/ && \
    git checkout 78ae759 && \
    ./bootstrap --parallel="$compile_cores" && \
    make -j"$compile_cores" && \
    make install && \
    alias cmake='/$WORKDIR/CMake/bin/cmake'

# get VacuumMesher
RUN cd /$WORKDIR && \
    git clone https://github.com/aurora-multiphysics/VacuumMesher.git && \
    cd VacuumMesher && \
    git checkout ${BUILD_GIT_SHA} &&\
    git submodule update --init --recursive && \
    mkdir build && \
    cd build && \
    cmake -DLIBMESH_DIR=/usr/local/lib ../ && \
    make -j "$compile_cores" 

RUN cd /$WORKDIR && \
    cd VacuumMesher/build && \
    ./test/runTests 





    
