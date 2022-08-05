#pragma once
#include <memory>
#include <string>
#include "gtest/gtest.h"
#include "libmesh/mesh.h"
#include "libmesh/libmesh.h"
#include "argsHelper.h"


class BasicTest : public ::testing::Test {
 protected:
    BasicTest()
        : initnull(true)
    {

    }

    void createLibmeshEnv()
    {
      int argc = 1;
      char ** argv = new char*[argc];
      argv[0] = my_argv[0];
      
      
      try
      {
        init = std::make_shared<libMesh::LibMeshInit>(argc, argv);
        initnull = (init == nullptr);
        if(!initnull){
          std::cout << "LibMesh init performed successfully" << std::endl;
        }
      }
      catch(const std::exception& e)
      {
        std::cout << "LibMesh init not performed successfuly\n"; 
        std::cerr << e.what() << '\n';
      }
      
      mesh = std::make_shared<libMesh::Mesh>(init->comm());
    }

    virtual void SetUp() override{};

    //Libmesh Initialisation Object  
    std::shared_ptr<libMesh::LibMeshInit> init = nullptr;
    
    //Libmesh Mesh object
    std::shared_ptr<libMesh::Mesh> mesh = nullptr;

    bool initnull;
};