#Chimera
# add_executable(chimeraSkinning chimera.cpp ${projectSrc})
# target_include_directories(chimeraSkinning PUBLIC ${libmesh_INCLUDE} ${CMAKE_SOURCE_DIR}/include)
# target_link_libraries(chimeraSkinning PUBLIC ${libmesh_LIBS})

# #Hypervapotron
# add_executable(hypervapotronSkinning hypervapotron.cpp ${projectSrc})
# message(STATUS ${projectSrc})
# target_include_directories(hypervapotronSkinning PRIVATE ${libmesh_INCLUDE} ${CMAKE_SOURCE_DIR}/include)
# target_link_libraries(hypervapotronSkinning ${libmesh_LIBS})

# #Cube
# add_executable(cubeSkinning cube.cpp ${projectSrc})
# target_include_directories(cubeSkinning PRIVATE ${libmesh_INCLUDE} ${CMAKE_SOURCE_DIR}/include)
# target_link_libraries(cubeSkinning ${libmesh_LIBS})

# #Main
# add_executable(argumentIn main.cpp ${projectSrc})
# target_include_directories(argumentIn PUBLIC ${libmesh_INCLUDE} ${CMAKE_SOURCE_DIR}/include)
# target_link_libraries(argumentIn PUBLIC ${libmesh_LIBS})

# #Skinning
# add_executable(skinner skinner.cpp ${projectSrc})
# target_include_directories(skinner PUBLIC ${libmesh_INCLUDE} ${CMAKE_SOURCE_DIR}/include)
# target_link_libraries(skinner PUBLIC ${libmesh_LIBS})


# #reskin
# add_executable(reskinner reskinner.cpp ${projectSrc})
# target_include_directories(reskinner PUBLIC ${libmesh_INCLUDE} ${CMAKE_SOURCE_DIR}/include)
# target_link_libraries(reskinner PUBLIC ${libmesh_LIBS})


# #block1
# add_executable(block1_test block1_tetra.cpp ${projectSrc})
# target_include_directories(block1_test PRIVATE ${libmesh_INCLUDE} ${CMAKE_SOURCE_DIR}/include)
# target_link_libraries(block1_test ${libmesh_LIBS} MPI::MPI_CXX igl::glfw igl::embree igl_copyleft::tetgen)

# #block2
# add_executable(block2_test block2_tetra.cpp ${projectSrc})
# target_include_directories(block2_test PRIVATE ${libmesh_INCLUDE} ${CMAKE_SOURCE_DIR}/include)
# target_link_libraries(block2_test ${libmesh_LIBS} MPI::MPI_CXX igl::glfw igl::embree igl_copyleft::tetgen)

# #block3
# add_executable(block3_test block3_tetra.cpp ${projectSrc})
# target_include_directories(block3_test PRIVATE ${libmesh_INCLUDE} ${CMAKE_SOURCE_DIR}/include)
# target_link_libraries(block3_test ${libmesh_LIBS} MPI::MPI_CXX igl::glfw igl::embree igl_copyleft::tetgen)

# #block5
# add_executable(block5_test block5_tetra.cpp ${projectSrc})
# target_include_directories(block5_test PRIVATE ${libmesh_INCLUDE} ${CMAKE_SOURCE_DIR}/include)
# target_link_libraries(block5_test ${libmesh_LIBS} MPI::MPI_CXX igl::glfw igl::embree igl_copyleft::tetgen)

# #block6
# add_executable(block6_test block6_tetra.cpp ${projectSrc})
# target_include_directories(block6_test PRIVATE ${libmesh_INCLUDE} ${CMAKE_SOURCE_DIR}/include)
# target_link_libraries(block6_test ${libmesh_LIBS} MPI::MPI_CXX igl::glfw igl::embree igl_copyleft::tetgen)

# #block6_0_subblock
# add_executable(block6_0_test block6_0_tetra.cpp ${projectSrc})
# target_include_directories(block6_0_test PRIVATE ${libmesh_INCLUDE} ${CMAKE_SOURCE_DIR}/include)
# target_link_libraries(block6_0_test ${libmesh_LIBS} MPI::MPI_CXX igl::glfw igl::embree igl_copyleft::tetgen)

# #block6_1_subblock
# add_executable(block6_1_test block6_1_tetra.cpp ${projectSrc})
# target_include_directories(block6_1_test PRIVATE ${libmesh_INCLUDE} ${CMAKE_SOURCE_DIR}/include)
# target_link_libraries(block6_1_test ${libmesh_LIBS} MPI::MPI_CXX igl::glfw igl::embree igl_copyleft::tetgen)


# #ChimeraNoLegs
# add_executable(chimera_noleg_test chimera_tetra.cpp ${projectSrc})
# target_include_directories(chimera_noleg_test PRIVATE ${libmesh_INCLUDE} ${CMAKE_SOURCE_DIR}/include)
# target_link_libraries(chimera_noleg_test ${libmesh_LIBS} MPI::MPI_CXX igl::glfw igl::embree igl_copyleft::tetgen)

# # theCube
# add_executable(cube_test cubeTet.cpp ${projectSrc})
# target_include_directories(cube_test PRIVATE ${libmesh_INCLUDE} ${CMAKE_SOURCE_DIR}/include)
# target_link_libraries(cube_test ${libmesh_LIBS} MPI::MPI_CXX igl::glfw igl::embree igl_copyleft::tetgen)

# # theSphere
# add_executable(sphere_test sphere.cpp ${projectSrc})
# target_include_directories(sphere_test PRIVATE ${libmesh_INCLUDE} ${CMAKE_SOURCE_DIR}/include)
# target_link_libraries(sphere_test ${libmesh_LIBS} MPI::MPI_CXX igl::glfw igl::embree igl_copyleft::tetgen)

#ligIGL normals
# add_executable(get_normals getNormal.cpp ${projectSrc})
# target_include_directories(get_normals PRIVATE ${libmesh_INCLUDE} ${CMAKE_SOURCE_DIR}/include)
# target_link_libraries(get_normals ${libmesh_LIBS} MPI::MPI_CXX igl::glfw igl::embree igl_copyleft::tetgen)


#The whole shabang
add_executable(makeVac makeVacuumMesh.cpp ${projectSrc})
target_include_directories(makeVac PRIVATE ${libmesh_INCLUDE} ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(makeVac ${libmesh_LIBS} MPI::MPI_CXX igl::glfw igl::embree igl_copyleft::tetgen)

#The whole shabang
# add_executable(libmeshData cubetest.cpp ${projectSrc})
# target_include_directories(libmeshData PRIVATE ${libmesh_INCLUDE} ${CMAKE_SOURCE_DIR}/include)
# target_link_libraries(libmeshData ${libmesh_LIBS} MPI::MPI_CXX igl::glfw igl::embree igl_copyleft::tetgen)

#Remove duplicate verts
add_executable(removeDupe removeDuplicates.cpp ${projectSrc})
target_include_directories(removeDupe PRIVATE ${libmesh_INCLUDE} ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(removeDupe ${libmesh_LIBS} MPI::MPI_CXX igl::glfw igl::embree igl_copyleft::tetgen)

#Generate 2D boundary
add_executable(generateBound 2dBound.cpp ${projectSrc})
target_include_directories(generateBound PRIVATE ${libmesh_INCLUDE} ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(generateBound ${libmesh_LIBS} MPI::MPI_CXX igl::glfw igl::embree igl_copyleft::tetgen igl_restricted::triangle)


