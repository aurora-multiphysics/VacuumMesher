#!/bin/bash

set -e

Help()
{
    # Display Help
    echo
    echo "Syntax: $0 [options]"
    echo "options:"
    echo "h      Print this help."
    echo "w      Set working directory"
    echo "j      Set number of compile cores"
    echo "d      Provide HDF5 installation directory"
    echo "f      Set MOOSE configuration flags as a string"
    echo "r      Must supply this if user is running as root (e.g. in docker)"
    echo "c      Allow petsc to download cmake"
    echo "p      Set libmesh install prefix"
    echo
}

# Defaults
WORKDIR=
TAG=""
BRANCH=""
GITSHA=""
JOBS=1
HDF5_DIR_IN=""
HDF5_STR=""
PETSC_INSTALL_DIR=""
ENV_FILE=""
ENV_OUTFILE=""
ROOTUSER=false
DOWNLOAD_CMAKE=false
LIBMESH_PREFIX_DIR=""
METHODS='opt dbg'

# Read arguments
while getopts "w:t:b:s:j:d:p:e:o:f:hrc" option; do
    case $option in
        h)
            Help
            exit;;
        w)
            WORKDIR=$(realpath $OPTARG);;
        t)
            TAG=$OPTARG;;
        b)
            BRANCH=$OPTARG;;
        s)
            GITSHA=$OPTARG;;
        j)
            JOBS=$OPTARG;;
        d)
            HDF5_DIR_IN=$(realpath $OPTARG);;
        e)
            ENV_FILE=$(realpath $OPTARG);;
        o)
            ENV_OUTFILE=$(realpath $OPTARG);;
        f)
            CONFIG_FLAGS=$OPTARG;;
        r)
            ROOTUSER=true;;
	p)
	    LIBMESH_PREFIX_DIR=$(realpath $OPTARG);;
        \?) # Invalid option
            echo "Error: Invalid option"
            exit 1;;
    esac
done

if [ "$WORKDIR" = "" ] ; then
    echo "Please set working directory through the -w flag"
    Help
    exit 1
fi

if [ -n "$HDF5_DIR_IN" ]; then
    echo "HDF5 installation location was set using HDF5_DIR=$HDF5_DIR_IN"
    HDF5_STR="--with-hdf5=$HDF5_DIR_IN"
fi

# Set up environment variables
export LIBMESH_DIR=$WORKDIR/libmesh
export LIBMESH_BUILD_DIR=$LIBMESH_DIR/build

# Create working directory if non-existant
echo "Installing LIBMESH in $WORKDIR"
if [ ! -d $WORKDIR ]; then
    mkdir $WORKDIR
fi
cd $WORKDIR

# Get libmesh repository if not present
if [ ! -d ${LIBMESH_DIR} ]; then
    git clone https://github.com/libMesh/libmesh
fi

cd $LIBMESH_DIR
# Make libmesh build repository if not present
if [ ! -d ${LIBMESH_BUILD_DIR} ]; then
    mkdir $LIBMESH_BUILD_DIR
fi

# Configure Libmesh
cd ${LIBMESH_BUILD_DIR}
git submodule update --init --recursive
../configure --enable-silent-rules \
               --enable-unique-id \
               --disable-warnings \
               --with-thread-model=openmp \
               --disable-maintainer-mode \
               --enable-hdf5 \
               --with-cxx-std-min=2017 \
               --without-gdb-command \
               --with-eigen \
               --with-methods="${METHODS}" \
               --prefix="${LIBMESH_PREFIX_DIR}" \
               --with-future-timpi-dir="${LIBMESH_PREFIX_DIR}" \
	       --disable-petsc \
	       "${HDF5_STR}"

make -j "${JOBS}" && make install

# This is needed or it mpiexec complains because docker runs as root
# Discussion on this issue https://github.com/open-mpi/ompi/issues/4451
if [ "$ROOTUSER" = true ] ; then
    export OMPI_ALLOW_RUN_AS_ROOT=1
    export OMPI_ALLOW_RUN_AS_ROOT_CONFIRM=1
fi

# Run tests
#cd ${LIBMESH_DIR}/test
#./run_tests -j${MOOSE_JOBS}

# Unset those OMPI environment variables we set before
if [ "$ROOTUSER" = true ] ; then
    export OMPI_ALLOW_RUN_AS_ROOT=
    export OMPI_ALLOW_RUN_AS_ROOT_CONFIRM=
fi

echo "Succesfully built Libmesh!"
echo
