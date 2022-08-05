#pragma once
#include "gtest/gtest.h"
#include"MeshTest.hpp"

class HypTest : public MeshTest 
{
 protected:
    HypTest() : MeshTest("../Meshes/hypervapotron.e"){} 

    virtual void SetUp() {MeshTest::SetUp();}

};

TEST_F(HypTest, firstTry)
{
    ASSERT_EQ(0, 0);
    ASSERT_EQ(mesh->n_elem(), 75613);
}