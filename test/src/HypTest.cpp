#pragma once
#include "gtest/gtest.h"
#include"MeshTest.hpp"

class HypTest : public MeshTest 
{
 protected:
    HypTest() : MeshTest("../hypervapotron.e"){} 

    virtual void SetUp() {MeshTest::SetUp();}

};

TEST_F(HypTest, firstTry)
{
    ASSERT_EQ(0, 0);
}