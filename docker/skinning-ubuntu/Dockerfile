# Get base image
FROM idaholab/moose:latest

# By default one core is used to compile
ARG compile_cores=6

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

#install moose
# RUN cd /$WORKDIR && \
#     git clone https://github.com/idaholab/moose.git && \
#     cd moose && \
#     git checkout master && \
#     MOOSE_JOBS="$compile_cores" && \
#     ./scripts/update_and_rebuild_petsc.sh && \
#     ./scripts/update_and_rebuild_libmesh.sh && \
#     cd /$WORKDIR/moose/test && \
#     make -j "$compile_cores"



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
    cmake -DFETCHCONTENT_SOURCE_DIR_LIBIGL=/opt/libigl -DMOOSE_DIR=/opt/moose ../ && \
    make -j "$compile_cores" && \
    cd /opt/libmeshSkinning && \
    ./build/tests/runTests





    
