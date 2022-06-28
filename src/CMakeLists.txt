add_executable(skin main.cpp surfaceMeshing.cpp)

target_include_directories(skin PRIVATE ${libmesh_INCLUDE} ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(skin ${libmesh_LIBS} MPI::MPI_CXX)