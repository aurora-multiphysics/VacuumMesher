# Get base image
FROM idaholab/moose:latest

# By default one core is used to compile
ARG compile_cores=2

# By default checkout mfem-moose branch
ARG WORKDIR="opt"
ARG build_git_sha="master"

RUN apt-get update -y && apt-get upgrade -y
# Install apt dependencies
RUN apt-get update && DEBIAN_FRONTEND=noninteractive TZ=Etc/UTC apt-get install -y \
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

# Build Catch2
RUN cd /$WORKDIR/submodules && \
    cd Catch2/ && \
    echo $("git branch") && \
    mkdir build && \
    cd build && \
    cmake ../ && \
    make -j "$compile_cores" && \
    cmake --install . 

# get VacuumMesher
RUN cd /$WORKDIR && \
    git clone https://github.com/aurora-multiphysics/VacuumMesher.git && \
    cd VacuumMesher && \
    git submodule update --init --recursive && \
    git checkout Catch2 && \
    mkdir build && \
    cd build && \
    cmake -DLIBMESH_DIR=/usr/local/lib ../ && \
    make -j "$compile_cores" 

RUN cd /$WORKDIR && \
    cd VacuumMesher/build && \
    ./test/runTests 





    
