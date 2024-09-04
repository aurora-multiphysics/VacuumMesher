ARG base_image_tag=latest
ARG libmesh_image_tag=latest

# Get base image
FROM billellis/base-ubuntu:$base_image_tag  AS base-ubuntu

# Copy files across
COPY ./scripts/build-libmesh.sh /home/scripts/

# By default 4 cores are used to compile
ARG compile_cores=4

# Create a local directory to install packages
RUN mkdir local

# Build all vmesh dependencies
RUN cd /home/scripts  && \
    HDF5_DIR=/usr/lib/x86_64-linux-gnu/hdf5/serial/ \
    ./build-libmesh.sh -w /home/ -p /usr/local/ -d /usr/lib/x86_64-linux-gnu/hdf5/serial/\
    -j "$compile_cores"

RUN cd /usr/local/lib/ && ldd libmesh_opt.so
    
# Now move to base ubuntu build image
FROM billellis/base-ubuntu:$base_image_tag

# Copy our dependencies and profiles across
COPY --from=base-ubuntu /usr/local /usr/local

RUN cd /usr/local/lib/ && ldd libmesh_opt.so
