#Chimera
add_executable(chimeraSkinning chimera.cpp ${projectSrc})
target_include_directories(chimeraSkinning PRIVATE ${libmesh_INCLUDE} ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(chimeraSkinning ${libmesh_LIBS} MPI::MPI_CXX igl::glfw igl::embree)

#Hypervapotron
add_executable(hypervapotronSkinning hypervapotron.cpp ${projectSrc})
message(STATUS ${projectSrc})
target_include_directories(hypervapotronSkinning PRIVATE ${libmesh_INCLUDE} ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(hypervapotronSkinning ${libmesh_LIBS} MPI::MPI_CXX igl::glfw igl::embree)

#Cube
add_executable(cubeSkinning cube.cpp ${projectSrc})
target_include_directories(cubeSkinning PRIVATE ${libmesh_INCLUDE} ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(cubeSkinning ${libmesh_LIBS} MPI::MPI_CXX igl::glfw igl::embree)

#Tetrahedralize
add_executable(tetrahedralize tetrahedra.cpp ${projectSrc})
target_include_directories(tetrahedralize PRIVATE ${libmesh_INCLUDE} ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(tetrahedralize ${libmesh_LIBS} MPI::MPI_CXX igl::glfw igl::embree igl_copyleft::tetgen)
