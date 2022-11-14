#!/bin/bash

search_dir="/home/bill/projects/libmesh-skinning/Meshes/chimBlocks"
for entry in "${search_dir}"/*.e
do 
    ./build/problems/skinner "$(basename "$entry")"
done


