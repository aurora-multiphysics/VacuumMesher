



# Link runTests with what we want to test and the GTest and pthread library
#message(STATUS ${libmesh_INCLUDE})
add_executable(runTests ${projectSrc} ${testSrc})
target_include_directories(runTests PRIVATE ${libmesh_INCLUDE} ${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/test/include)
target_compile_definitions(runTests PUBLIC USER_DIR="${CMAKE_SOURCE_DIR}")
target_link_libraries(runTests Catch2::Catch2WithMain pthread MPI::MPI_CXX VacuumMeshing mesh_opt)
