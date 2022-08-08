#pragma once
#include "gtest/gtest.h"
#include"SkinningTest.hpp"

class HypTest : public SkinningTest 
{
 protected:
    HypTest() : SkinningTest("hypervapotron.e"){} 

    virtual void SetUp() {SkinningTest::SetUp();}
};

TEST_F(HypTest, meshReadSuccessful)
{
    ASSERT_EQ(0, 0);
    ASSERT_EQ(mesh->n_elem(), 75613);
    ASSERT_EQ(cubitSurfaceMesh->n_elem(), surfaceMesh->n_elem());
    ASSERT_EQ(cubitSurfaceMesh->n_node(), surfaceMesh->n_node());

}


