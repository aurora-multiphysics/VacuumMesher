#!/bin/bash

set -e

# Update package lists to get latest cmake
apt-get update
apt-get purge --auto-remove cmake
apt-get -y --fix-missing install \
        software-properties-common \
        lsb-release \
        gpg \
        wget
apt-get clean all
apt-get update
apt-get install -y --fix-missing cmake \
        gcc \
        g++ \
        gfortran \
        git \
        libopenmpi3 \
        python3 \
        python3-venv \
        python3-dev \
        python3-distutils \
        python-is-python3 \
        rsync \
        bison \
        flex \
        libblas-dev \
        liblapack-dev \
        libhdf5-dev \
        libeigen3-dev \
        libglfw3-dev  \
        libtbb-dev  \
        pkg-config
