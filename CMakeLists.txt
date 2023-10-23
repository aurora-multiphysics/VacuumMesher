cmake_minimum_required(VERSION 3.01)

project(LibMeshSkinning)
# CMAKE build type to optimised code with debug symbols
set(CMAKE_BUILD_TYPE Release)
add_compile_options(-std=c++17)

#Check if MOOSE_DIR is defined
if(NOT DEFINED LIBMESH_DIR)
    message(SEND_ERROR "Please define your the directory of your libmesh config executable")
endif()
set(libmesh_INCLUDE ${LIBMESH_DIR}/include)
link_directories (${LIBMESH_DIR}/lib/)

#Get MPI packages
find_package(MPI REQUIRED)


# Get libmesh includes and libs
# execute_process(COMMAND ${LIBMESH_CONF_DIR}/libmesh-config --libs OUTPUT_VARIABLE libmesh_LIBS OUTPUT_STRIP_TRAILING_WHITESPACE)
# execute_process(COMMAND ${LIBMESH_CONF_DIR}/libmesh-config --include OUTPUT_VARIABLE libmesh_INCLUDE OUTPUT_STRIP_TRAILING_WHITESPACE)

# Get rid of "-I" from libmesh_INCLUDE string
# string(REPLACE "-I" "" libmesh_INCLUDE "${libmesh_INCLUDE}")
# string(REPLACE " " ";" libmesh_INCLUDE "${libmesh_INCLUDE}")
# set(libmesh_INCLUDEWORKING /home/bill/projects/moose/scripts/../libmesh/installed/include /home/bill/projects/moose/petsc/arch-moose/include /home/bill/projects/moose/petsc/include /usr/include /usr/include/tirpc)
# string(STRIP "${libmesh_INCLUDE}" libmesh_INCLUDE)
# set(libmesh_INCLUDE ${MOOSE_DIR}/libmesh/installed/include 
#     ${MOOSE_DIR}/libmesh/installed/include/libmesh 
#     ${MOOSE_DIR}/petsc/arch-moose/include/ 
#     ${MOOSE_DIR}/petsc/include 
#     /usr/include/tirpc)

FILE(GLOB_RECURSE projectSrc ./src/*)
FILE(GLOB_RECURSE testSrc ./test/src/*)

add_subdirectory(VacuumMeshing)
add_subdirectory(./bin)
add_subdirectory(./test)
add_subdirectory(./submodules/Catch2)