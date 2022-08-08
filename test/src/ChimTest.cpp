#pragma once
#include "gtest/gtest.h"
#include"SkinningTest.hpp"

class ChimTest : public SkinningTest 
{
 protected:
    ChimTest() : SkinningTest("chimeraCsut.e"){} 

    virtual void SetUp() {SkinningTest::SetUp();}
};

TEST_F(ChimTest, meshReadSuccessful)
{
    ASSERT_EQ(0, 0);
    ASSERT_EQ(cubitSurfaceMesh->n_elem(), surfaceMesh->n_elem());
    ASSERT_EQ(cubitSurfaceMesh->n_nodes(), surfaceMesh->n_nodes());

    for(int node_num = 0; node_num < cubitSurfaceMesh->n_nodes(); node_num++)
    {
        ASSERT_EQ(surfaceNodes[node_num], cubSurfaceNodes[node_num]);
    }
    std::cout << cubSurfaceNodes.size() << "  " << surfaceNodes.size() << std::endl;
}


