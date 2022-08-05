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


#Tetrahedralize
# add_executable(tetMaker tetrahedra.cpp ${projectSrc})
# target_include_directories(tetMaker PRIVATE ${libmesh_INCLUDE} ${CMAKE_SOURCE_DIR}/include)
# target_link_libraries(tetMaker ${libmesh_LIBS} MPI::MPI_CXX igl::glfw igl::embree igl_copyleft::tetgen)
