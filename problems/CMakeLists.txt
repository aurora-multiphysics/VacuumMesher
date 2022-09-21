#Chimera
add_executable(chimeraSkinning chimera.cpp ${projectSrc})
target_include_directories(chimeraSkinning PUBLIC ${libmesh_INCLUDE} ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(chimeraSkinning PUBLIC ${libmesh_LIBS})

#Hypervapotron
add_executable(hypervapotronSkinning hypervapotron.cpp ${projectSrc})
message(STATUS ${projectSrc})
target_include_directories(hypervapotronSkinning PRIVATE ${libmesh_INCLUDE} ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(hypervapotronSkinning ${libmesh_LIBS})

#Cube
add_executable(cubeSkinning cube.cpp ${projectSrc})
target_include_directories(cubeSkinning PRIVATE ${libmesh_INCLUDE} ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(cubeSkinning ${libmesh_LIBS})

#Main
add_executable(argumentIn main.cpp ${projectSrc})
target_include_directories(argumentIn PUBLIC ${libmesh_INCLUDE} ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(argumentIn PUBLIC ${libmesh_LIBS})

#Skinning
add_executable(skinner skinner.cpp ${projectSrc})
target_include_directories(skinner PUBLIC ${libmesh_INCLUDE} ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(skinner PUBLIC ${libmesh_LIBS})


#reskin
add_executable(reskinner reskinner.cpp ${projectSrc})
target_include_directories(reskinner PUBLIC ${libmesh_INCLUDE} ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(reskinner PUBLIC ${libmesh_LIBS})


#block1
add_executable(block1_test block1_tetra.cpp ${projectSrc})
target_include_directories(block1_test PRIVATE ${libmesh_INCLUDE} ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(block1_test ${libmesh_LIBS} MPI::MPI_CXX igl::glfw igl::embree igl_copyleft::tetgen)

#block2
add_executable(block2_test block2_tetra.cpp ${projectSrc})
target_include_directories(block2_test PRIVATE ${libmesh_INCLUDE} ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(block2_test ${libmesh_LIBS} MPI::MPI_CXX igl::glfw igl::embree igl_copyleft::tetgen)

#block3
add_executable(block3_test block3_tetra.cpp ${projectSrc})
target_include_directories(block3_test PRIVATE ${libmesh_INCLUDE} ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(block3_test ${libmesh_LIBS} MPI::MPI_CXX igl::glfw igl::embree igl_copyleft::tetgen)

#block5
add_executable(block5_test block5_tetra.cpp ${projectSrc})
target_include_directories(block5_test PRIVATE ${libmesh_INCLUDE} ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(block5_test ${libmesh_LIBS} MPI::MPI_CXX igl::glfw igl::embree igl_copyleft::tetgen)

#block6
add_executable(block6_test block6_tetra.cpp ${projectSrc})
target_include_directories(block6_test PRIVATE ${libmesh_INCLUDE} ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(block6_test ${libmesh_LIBS} MPI::MPI_CXX igl::glfw igl::embree igl_copyleft::tetgen)


# theCube
add_executable(cube_test cubeTet.cpp ${projectSrc})
target_include_directories(cube_test PRIVATE ${libmesh_INCLUDE} ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(cube_test ${libmesh_LIBS} MPI::MPI_CXX igl::glfw igl::embree igl_copyleft::tetgen)
