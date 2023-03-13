 
# Link runTests with what we want to test and the GTest and pthread library
#message(STATUS ${libmesh_INCLUDE})
add_executable(runTests ${projectSrc} ${testSrc})
target_include_directories(runTests PRIVATE ${libmesh_INCLUDE} ${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/test/include ${GTEST_INCLUDE_DIRS})
target_link_libraries(runTests ${GTEST_LIBRARIES} pthread MPI::MPI_CXX VacuumMeshing mesh_opt)
