
#Skinning
add_executable(skin skin.cpp /home/bill/Projects/VacuumMesher/VacuumMeshing/src/SurfaceMeshing/SurfaceGenerator.cpp)
target_include_directories(skin PUBLIC ${libmesh_INCLUDE} ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(skin PUBLIC MPI::MPI_CXX VacuumMeshing mesh_opt) 

#Full Run
# add_executable(vmesh vmesh.cpp ${projectSrc})
# target_include_directories(vmesh PUBLIC ${libmesh_INCLUDE} ${CMAKE_SOURCE_DIR}/include )
# target_link_libraries(vmesh PUBLIC MPI::MPI_CXX VacuumMeshing mesh_opt) 

# #Example
# add_executable(vcoil vcoil.cpp ${projectSrc})
# target_include_directories(vcoil PUBLIC ${CMAKE_SOURCE_DIR}/include ${libmesh_INCLUDE})
# target_link_libraries(vcoil PUBLIC MPI::MPI_CXX mesh_opt VacuumMeshing) 

add_executable(skin2 skin2.cpp /home/bill/Projects/VacuumMesher/VacuumMeshing/src/SurfaceMeshing/SurfaceGenerator.cpp)
target_include_directories(skin2 PUBLIC ${libmesh_INCLUDE} ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(skin2 PUBLIC MPI::MPI_CXX VacuumMeshing mesh_opt) 

add_executable(boundaryGen boundary.cpp /home/bill/Projects/VacuumMesher/VacuumMeshing/src/SurfaceMeshing/SurfaceGenerator.cpp /home/bill/Projects/VacuumMesher/VacuumMeshing/src/BoundaryGeneration/BoundaryGenerator.cpp)
target_include_directories(boundaryGen PUBLIC ${libmesh_INCLUDE} ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(boundaryGen PUBLIC MPI::MPI_CXX VacuumMeshing mesh_opt) 