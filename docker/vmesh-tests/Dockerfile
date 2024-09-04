ARG base_image_tag=latest

# Get base image
FROM billellis/vmesh-deps-ubuntu:$base_image_tag

# By default one core is used to compile
ARG compile_cores=8

ARG WORKDIR="opt"
ARG BUILD_GIT_SHA
RUN echo ${BUILD_GIT_SHA}
RUN apt update -y && apt upgrade -y

ENV LD_LIBRARY_PATH=/usr/local/lib/
ENV export LD_LIBRARY_PATH=$LD_LIBRARY_PATH
RUN echo $LD_LIBRARY_PATH

# Clone vacuummesher repo and build it 
RUN cd /$WORKDIR && \
    git clone https://github.com/aurora-multiphysics/VacuumMesher.git && \
    cd VacuumMesher && \
    git checkout ${BUILD_GIT_SHA} && \
    git submodule update --init --recursive && \
    mkdir build && \
    cd build && \
    cmake -DLIBMESH_DIR=/usr/local/ ../ && \
    make -j "$compile_cores" 

RUN cd /$WORKDIR && \
    cd VacuumMesher/build && \
    ./test/runTests 





    
