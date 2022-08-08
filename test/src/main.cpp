#include <fstream>
#include <string>
#include "LoadUnitsTests.h"
#include "argsHelper.h"
#include "gtest/gtest.h"


int my_argc;
char** my_argv;


GTEST_API_ int main(int argc, char** argv) {
  my_argc = argc;
  my_argv = argv;

  if(!SkinningUnitTestsLoaded())
  {
    std::cout << "Unit tests not loaded" << std::endl;
    return -1;
  }

  testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}