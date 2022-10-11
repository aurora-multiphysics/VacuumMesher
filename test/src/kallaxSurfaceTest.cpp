
#include"SkinningTest.hpp"

class KallaxSurfaceTest : public SkinningTest 
{
 protected:
    KallaxSurfaceTest() : SkinningTest("Kallax.e"){} 

    virtual void OverRide() {BasicTest::TearDown();}
};


TEST_F(KallaxSurfaceTest, checkNumNodes)
{
    ASSERT_EQ(surfaceMesh->n_nodes(), 2038);
}

TEST_F(KallaxSurfaceTest, checkNumElem)
{
    ASSERT_EQ(surfaceMesh->n_elem(), 4056);
}





