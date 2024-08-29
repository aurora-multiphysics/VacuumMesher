ARG base_image_tag=latest

# Get base image
FROM billellis/vmesh-ubuntu:$base_image_tag

# By default one core is used to compile
ARG compile_cores=2

ARG WORKDIR="/home/"

RUN cd /$WORKDIR && \
    cd VacuumMesher/build && \
    ./test/runTests 





    
