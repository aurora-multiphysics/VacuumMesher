#include <fstream>
#include <string>
#include "LoadUnitsTests.h"
#include "gtest/gtest.h"

GTEST_API_ int main(int argc, char** argv) {
  // gtest removes (only) its args from argc and argv - so this  must be before
  // moose init
  testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}