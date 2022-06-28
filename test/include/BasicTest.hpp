#pragma once
#include <memory>
#include <string>
#include "gtest/gtest.h"
#include "libmesh/mesh.h"
#include "libmesh/libmesh.h"

using namespace libMesh;

class BasicTest : public ::testing::Test {
 protected:
    BasicTest()
        : args(""){};

  void createLibmeshEnv()
  {
    //Turn string arguments into c-style string
    char* cstr = new char[args.length() + 1];
    std::strcpy(cstr, args.c_str());

    // Split string by whitespace delimiter
    std::vector<char*> arg_list;
    char* next;
    next = strtok(cstr, " ");
    while (next != NULL) {
      arg_list.push_back(next);
      next = strtok(NULL, " ");
    }
    
    // Create array of char*
    int argc = (int)(arg_list.size());
    // char** argv = new char*[argc];
    // for (size_t i = 0; i < argc; i++) {
    //   argv[i] = arg_list.at(i);
    // }
    std::vector<std::string> arguments = {""};
    std::vector<char*> argv;
    for (const auto& arg : arguments)
    {
      argv.push_back((char*)arg.data());
    }
    argv.push_back(nullptr);
    std::cout <<"libmesh init not working  " << argc << " " << argv[0] << std::endl;
    init = new LibMeshInit(argc, argv.data()); 
    std::cout << "o" << std::endl;
    mesh = new Mesh(init->comm());
    std::cout << "mesh created" << std::endl;

    // delete argv;
    delete cstr;
  }

  virtual void SetUp() override{};

  // Arguments for our app
  std::string args;

  //Libmesh Initialisation Object  
  LibMeshInit* init;
  
  //Libmesh Mesh object
  Mesh* mesh;
};