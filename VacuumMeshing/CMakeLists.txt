# cmakelists to build lib

#libigl
option(LIBIGL_INSTALL            "Enable installation of libigl targets" ON)

# libigl module options
option(LIBIGL_EMBREE              "Build target igl::embree"              ON)
option(LIBIGL_GLFW                "Build target igl::glfw"                OFF)
option(LIBIGL_IMGUI               "Build target igl::imgui"               OFF)
option(LIBIGL_OPENGL              "Build target igl::opengl"              OFF)
option(LIBIGL_PNG                 "Build target igl::png"                 OFF)
option(LIBIGL_PREDICATES          "Build target igl::predicates"          OFF)
option(LIBIGL_XML                 "Build target igl::xml"                 OFF)
option(LIBIGL_COPYLEFT_CGAL       "Build target igl_copyleft::cgal"       OFF)
option(LIBIGL_COPYLEFT_COMISO     "Build target igl_copyleft::comiso"     OFF)
option(LIBIGL_COPYLEFT_CORE       "Build target igl_copyleft::core"       ON)
option(LIBIGL_COPYLEFT_CORK       "Build target igl_copyleft::cork"       OFF)
option(LIBIGL_COPYLEFT_TETGEN     "Build target igl_copyleft::tetgen"     ON)
option(LIBIGL_RESTRICTED_TRIANGLE "Build target igl_restricted::triangle" ON)

add_subdirectory(libigl)

FILE(GLOB_RECURSE libSrc  ${CMAKE_CURRENT_SOURCE_DIR}/src/*)

#Build release library
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_library(vmesh_opt SHARED ${libSrc})
    target_link_libraries(vmesh_opt PRIVATE MPI::MPI_CXX mesh_opt PUBLIC timpi_opt igl::embree igl_copyleft::tetgen igl_restricted::triangle)
    target_include_directories(vmesh_opt PRIVATE ${libmesh_INCLUDE} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
endif()

#Build debug library
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_library(vmesh_dbg SHARED ${libSrc})
    target_link_libraries(vmesh_dbg PRIVATE MPI::MPI_CXX mesh_dbg PUBLIC timpi_opt igl::embree igl_copyleft::tetgen igl_restricted::triangle)
    target_include_directories(vmesh_dbg PRIVATE ${libmesh_INCLUDE} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
endif()

