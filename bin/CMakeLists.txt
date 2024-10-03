#Skinning
add_executable(skin skin.cpp ${projectSrc})
target_include_directories(skin PUBLIC ${libmesh_INCLUDE} ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(skin PUBLIC MPI::MPI_CXX vmesh_opt mesh_opt) 


#Boundary Generation
add_executable(boundary boundary.cpp ${projectSrc})
target_include_directories(boundary PUBLIC ${libmesh_INCLUDE} ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(boundary PUBLIC MPI::MPI_CXX vmesh_opt mesh_opt) 


#Coil Boundary Generation
add_executable(coilBoundary coil_boundary.cpp ${projectSrc})
target_include_directories(coilBoundary PUBLIC ${libmesh_INCLUDE} ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(coilBoundary PUBLIC MPI::MPI_CXX vmesh_opt mesh_opt) 

#Vacuum Mesh Generation
add_executable(vacuum vacuum.cpp ${projectSrc})
target_include_directories(vacuum PUBLIC ${libmesh_INCLUDE} ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(vacuum PUBLIC MPI::MPI_CXX vmesh_opt mesh_opt) 

#Coil Vacuum Mesh Generation
add_executable(coilVacuum coil_vacuum.cpp ${projectSrc})
target_include_directories(coilVacuum PUBLIC ${libmesh_INCLUDE} ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(coilVacuum PUBLIC MPI::MPI_CXX vmesh_opt mesh_opt) 

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_executable(skin_dbg skin.cpp ${projectSrc})
    target_include_directories(skin_dbg PUBLIC ${libmesh_INCLUDE} ${CMAKE_SOURCE_DIR}/include)
    target_link_libraries(skin_dbg PUBLIC MPI::MPI_CXX vmesh_dbg mesh_dbg) 

    add_executable(vacuum_dbg vacuum.cpp ${projectSrc})
    target_include_directories(vacuum_dbg PUBLIC ${libmesh_INCLUDE} ${CMAKE_SOURCE_DIR}/include)
    target_link_libraries(vacuum_dbg PUBLIC MPI::MPI_CXX vmesh_dbg mesh_dbg) 
endif()