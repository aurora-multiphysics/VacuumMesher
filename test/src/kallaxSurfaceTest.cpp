
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

TEST_F(KallaxSurfaceTest, checkNumElems)
{
    ASSERT_EQ(surfaceMesh->n_elem(), 4056) << "Number of elements does not align with verified mesh";
}

TEST_F(KallaxSurfaceTest, check2DElems)
{
    EXPECT_TRUE(checkAll2DElem()) << "Element types are not 2D, or are of unknown type";
}

