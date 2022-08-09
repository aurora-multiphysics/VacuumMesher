#include <fstream>
#include <string>
#include "argsHelper.h"
#include "initer.h"
#include "gtest/gtest.h"


int my_argc;
char** my_argv;
libMesh::LibMeshInit* initLibMsh;

GTEST_API_ int main(int argc, char** argv) {
  my_argc = argc;
  my_argv = argv;
  initLibMsh = new libMesh::LibMeshInit(argc, argv);

  testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}