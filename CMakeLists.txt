cmake_minimum_required(VERSION 3.01)

project(LibMeshSkinning)

# Locate GTest
find_package(GTest REQUIRED)
find_package(MPI REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

set(prefix /home/bill/projects/moose)
set(libmesh_LIBS "-Wl,-rpath,/home/bill/projects/moose/scripts/../libmesh/installed/lib -L/home/bill/projects/moose/scripts/../libmesh/installed/lib -lmesh_opt -ltimpi_opt -Wl,-rpath,/home/bill/projects/moose/petsc/arch-moose/lib -L/home/bill/projects/moose/petsc/arch-moose/lib -L/lib -Wl,-rpath,/lib -Wl,-rpath,/usr/lib/x86_64-linux-gnu/openmpi/lib/fortran/gfortran -L/usr/lib/x86_64-linux-gnu/openmpi/lib/fortran/gfortran -Wl,-rpath,/usr/lib/gcc/x86_64-linux-gnu/11 -L/usr/lib/gcc/x86_64-linux-gnu/11 -lslepc -lpetsc -lHYPRE -lstrumpack -lscalapack -lsuperlu_dist -lflapack -lfblas -lptesmumps -lptscotchparmetis -lptscotch -lptscotcherr -lesmumps -lscotch -lscotcherr -lhdf5_hl -lhdf5 -lparmetis -lmetis -lX11 -lmpi_usempif08 -lmpi_usempi_ignore_tkr -lmpi_mpifh -lmpi -lopen-rte -lopen-pal -lhwloc -levent_core -levent_pthreads -lz -lm -lgfortran -lgcc_s -lrt -lquadmath -lstdc++ -ldl -ltirpc")
set(libmesh_INCLUDE ${prefix}/libmesh/installed/include 
    ${prefix}/libmesh/installed/include/libmesh 
    ${prefix}/petsc/arch-moose/include/ 
    ${prefix}/petsc/include 
    /usr/include/tirpc)

FILE(GLOB projectSrc ./src/*)
FILE(GLOB testSrc ./test/src/*)

add_subdirectory(./problems)
add_subdirectory(./test)