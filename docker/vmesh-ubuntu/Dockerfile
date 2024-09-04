ARG base_image_tag=latest

# Get base image
FROM billellis/vmesh-deps-ubuntu:$base_image_tag  AS vmesh-libmesh-base

# By default 4 cores are used to compile
ARG compile_cores=4

# Create a local directory to install packages
RUN mkdir local

# Go into local
RUN cd local

#Git clone and build VacuumMesher

RUN git clone https://github.com/aurora-multiphysics/VacuumMesher.git
RUN cd VacuumMesher && git submodule update --init --recursive && mkdir build && cd build && cmake -DLIBMESH_DIR=/usr/local/ ../ && make -j "$compile_cores"

    

