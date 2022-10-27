cmake_minimum_required(VERSION 3.01)

project(LibMeshSkinning)

add_compile_options(-O3)

#Project Path
list(PREPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

#libigl
# option(LIBIGL_BUILD_TESTS        "Build libigl unit test"                OFF)
# option(LIBIGL_BUILD_TUTORIALS    "Build libigl tutorial"                 OFF)
option(LIBIGL_INSTALL            "Enable installation of libigl targets" ON)
# option(LIBIGL_USE_STATIC_LIBRARY "Use libigl as static library"          OFF)

# Module options
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
find_package(GTest REQUIRED)
find_package(MPI REQUIRED)

include_directories(${GTEST_INCLUDE_DIRS})

set(prefix /home/bill/projects/moose)
set(libmesh_LIBS "-Wl,-rpath,/home/bill/projects/moose/scripts/../libmesh/installed/lib -L/home/bill/projects/moose/scripts/../libmesh/installed/lib -lmesh_opt -ltimpi_opt -Wl,-rpath,/home/bill/projects/moose/petsc/arch-moose/lib -L/home/bill/projects/moose/petsc/arch-moose/lib -L/lib -Wl,-rpath,/lib -Wl,-rpath,/usr/lib/x86_64-linux-gnu/openmpi/lib/fortran/gfortran -L/usr/lib/x86_64-linux-gnu/openmpi/lib/fortran/gfortran -Wl,-rpath,/usr/lib/gcc/x86_64-linux-gnu/11 -L/usr/lib/gcc/x86_64-linux-gnu/11 -lslepc -lpetsc -lHYPRE -lcmumps -ldmumps -lsmumps -lzmumps -lmumps_common -lpord -lstrumpack -lscalapack -lsuperlu_dist -lflapack -lfblas -lptesmumps -lptscotchparmetis -lptscotch -lptscotcherr -lesmumps -lscotch -lscotcherr -lhdf5_hl -lhdf5 -lparmetis -lmetis -lmpi_usempif08 -lmpi_usempi_ignore_tkr -lmpi_mpifh -lmpi -lopen-rte -lopen-pal -lhwloc -levent_core -levent_pthreads -lz -lm -lgfortran -lgcc_s -lrt -lquadmath -lstdc++ -ldl -ltirpc")
set(libmesh_INCLUDE ${prefix}/libmesh/installed/include 
    ${prefix}/libmesh/installed/include/libmesh 
    ${prefix}/petsc/arch-moose/include/ 
    ${prefix}/petsc/include 
    /usr/include/tirpc)

FILE(GLOB projectSrc ./src/*)
FILE(GLOB testSrc ./test/src/*.cpp)
message(STATUS ${projectSrc})
link_libraries(MPI::MPI_CXX igl::glfw igl::embree igl_copyleft::cgal igl_copyleft::tetgen igl_restricted::triangle igl::predicates)

add_subdirectory(./problems)
add_subdirectory(./test)