#pragma once
#include <memory>
#include <string>
#include "gtest/gtest.h"
#include "libmesh/mesh.h"
#include "libmesh/libmesh.h"
#include "argsHelper.h"
#include "initer.h"

class BasicTest : public ::testing::Test {
 protected:
    BasicTest()
    {

    }

    virtual void SetUp() override{};

    // virtual void TearDown() override
    // {
    //   init = nullptr;  
    // } 

    // //Libmesh Initialisation Object  
    // std::shared_ptr<libMesh::LibMeshInit> init = nullptr;
  
    // bool initnull;
};