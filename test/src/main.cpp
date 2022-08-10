#include <fstream>
#include <string>
#include "initer.h"
#include "gtest/gtest.h"


libMesh::LibMeshInit* init;

GTEST_API_ int main(int argc, char** argv) {

  init = new libMesh::LibMeshInit(argc, argv);

  testing::InitGoogleTest(&argc, argv);

  int ret = RUN_ALL_TESTS();

  delete(init);

  return ret;
}