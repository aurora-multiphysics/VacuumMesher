cmake_minimum_required(VERSION 3.01)

project(LibMeshSkinning)

add_compile_options(-std=c++17)

#Project Path
list(PREPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

#Check if MOOSE_DIR is defined
if(NOT DEFINED MOOSE_DIR)
    message(SEND_ERROR "Please define your MOOSE dir")
endif()

#Get gtest and MPI packages
find_package(GTest REQUIRED)
find_package(MPI REQUIRED)

#libigl
option(LIBIGL_INSTALL            "Enable installation of libigl targets" ON)

# libigl module options
option(LIBIGL_EMBREE              "Build target igl::embree"              ON)
option(LIBIGL_GLFW                "Build target igl::glfw"                ON)
option(LIBIGL_IMGUI               "Build target igl::imgui"               ON)
option(LIBIGL_OPENGL              "Build target igl::opengl"              ON)
option(LIBIGL_PNG                 "Build target igl::png"                 ON)
option(LIBIGL_PREDICATES          "Build target igl::predicates"          ON)
option(LIBIGL_XML                 "Build target igl::xml"                 ON)
option(LIBIGL_COPYLEFT_CGAL       "Build target igl_copyleft::cgal"       ON)
option(LIBIGL_COPYLEFT_COMISO     "Build target igl_copyleft::comiso"     ON)
option(LIBIGL_COPYLEFT_CORE       "Build target igl_copyleft::core"       ON)
option(LIBIGL_COPYLEFT_CORK       "Build target igl_copyleft::cork"       ON)
option(LIBIGL_COPYLEFT_TETGEN     "Build target igl_copyleft::tetgen"     ON)
option(LIBIGL_RESTRICTED_TRIANGLE "Build target igl_restricted::triangle" ON)

include(libigl)
# Locate GTest and MPI

execute_process(COMMAND ${MOOSE_DIR}/libmesh/installed/bin/libmesh-config --libs OUTPUT_VARIABLE libmesh_LIBS OUTPUT_STRIP_TRAILING_WHITESPACE)

set(libmesh_INCLUDE ${MOOSE_DIR}/libmesh/installed/include 
    ${MOOSE_DIR}/libmesh/installed/include/libmesh 
    ${MOOSE_DIR}/petsc/arch-moose/include/ 
    ${MOOSE_DIR}/petsc/include 
    /usr/include/tirpc)

FILE(GLOB projectSrc ./src/*)
FILE(GLOB testSrc ./test/src/*)

link_libraries(MPI::MPI_CXX igl::glfw igl::embree igl_copyleft::cgal igl_copyleft::tetgen igl_restricted::triangle igl::predicates)

add_subdirectory(./problems)
add_subdirectory(./test)