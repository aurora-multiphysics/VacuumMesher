ARG base_image_tag=latest

# Get base image
FROM billellis/vmesh-deps-ubuntu:$base_image_tag

# By default one core is used to compile
ARG compile_cores=2

ARG WORKDIR="opt"
ARG BUILD_GIT_SHA
RUN echo ${BUILD_GIT_SHA}
RUN apt update -y && apt upgrade -y

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





    
