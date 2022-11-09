#include"SkinningTest.hpp"

class cubeSkinningTest : public SkinningTest 
{
 protected:
    cubeSkinningTest() : SkinningTest("unit_cube.e"){} 

    virtual void OverRide() {BasicTest::TearDown();}
};

TEST_F(cubeSkinningTest, checkNumNodes)
{
    ASSERT_EQ(surfaceMesh->n_nodes(), 704);
}

TEST_F(cubeSkinningTest, checkNumElems)
{
    ASSERT_EQ(surfaceMesh->n_elem(), 1404) << "Number of elements does not align with verified mesh";
}

TEST_F(cubeSkinningTest, checkNodePositions)
{
    refSurfaceMesh->read(refSurfaceFilepath);
    for(int node = 0; node < surfaceMesh->n_nodes(); node++)
    {
        ASSERT_EQ(surfaceMesh->node_ref(node), refSurfaceMesh->node_ref(node));
    }
}

TEST_F(cubeSkinningTest, check2DElems)
{
    for(auto& elem: surfaceMesh->element_ptr_range())
    {
        EXPECT_LT(elem->type(), 7) << "Not all elements are 2D, as would be expected from a skinned mesh";
    }
}