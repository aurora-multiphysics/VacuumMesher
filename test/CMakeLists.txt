 
# Locate GTest

FILE(GLOB mySrc /home/bill/projects/libmeshTest/test/src/*)
set(myIncludeDir /home/bill/projects/libmeshTest/test/include)
# Link runTests with what we want to test and the GTest and pthread library
message(STATUS ${libmesh_INCLUDE})
add_executable(runTests ${mySrc})
target_include_directories(runTests PRIVATE ${libmesh_INCLUDE} ${myIncludeDir})
target_link_libraries(runTests ${GTEST_LIBRARIES} pthread ${libmesh_LIBS} MPI::MPI_CXX)