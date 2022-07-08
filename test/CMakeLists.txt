 
# Link runTests with what we want to test and the GTest and pthread library
#message(STATUS ${libmesh_INCLUDE})
add_executable(runTests ${projectSrc} ${testSrc})
target_include_directories(runTests PRIVATE ${libmesh_INCLUDE} ${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/test/include)
target_link_libraries(runTests ${GTEST_LIBRARIES} pthread ${libmesh_LIBS} MPI::MPI_CXX igl::glfw igl::embree)
