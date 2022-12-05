
#Skinning
add_executable(skinner skinMesh.cpp ${projectSrc})
target_include_directories(skinner PUBLIC ${libmesh_INCLUDE} ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(skinner PUBLIC ${libmesh_LIBS} MPI::MPI_CXX igl::glfw igl::embree igl_copyleft::cgal igl_copyleft::tetgen igl_restricted::triangle igl::predicates)

#The whole shabang
add_executable(makeVac genVacFromSurf.cpp ${projectSrc})
target_include_directories(makeVac PRIVATE ${libmesh_INCLUDE} ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(makeVac PUBLIC ${libmesh_LIBS} MPI::MPI_CXX igl::glfw igl::embree igl_copyleft::cgal igl_copyleft::tetgen igl_restricted::triangle igl::predicates)

# Mesh boolean
add_executable(meshBool meshBooleanOperations.cpp ${projectSrc})
target_include_directories(meshBool PRIVATE ${libmesh_INCLUDE} ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(meshBool PUBLIC ${libmesh_LIBS} MPI::MPI_CXX igl::glfw igl::embree igl_copyleft::cgal igl_copyleft::tetgen igl_restricted::triangle igl::predicates)

#Full Run
add_executable(fullRun fullRun.cpp ${projectSrc})
target_include_directories(fullRun PRIVATE ${libmesh_INCLUDE} ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(fullRun PUBLIC ${libmesh_LIBS} MPI::MPI_CXX igl::glfw igl::embree igl_copyleft::cgal igl_copyleft::tetgen igl_restricted::triangle igl::predicates)

#fullCoil
add_executable(fullCoil coilRun.cpp ${projectSrc})
target_include_directories(fullCoil PRIVATE ${libmesh_INCLUDE} ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(fullCoil PUBLIC ${libmesh_LIBS} MPI::MPI_CXX igl::glfw igl::embree igl_copyleft::cgal igl_copyleft::tetgen igl_restricted::triangle igl::predicates)