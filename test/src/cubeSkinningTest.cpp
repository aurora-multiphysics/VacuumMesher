#include"SkinningTest.hpp"

class cubeSkinningTest : public SkinningTest 
{
 protected:
    cubeSkinningTest() : SkinningTest("testingMeshes/unit_cube.e")
    {
        refSkinnedMesh->read("testingMeshes/unit_cube_ref.e");
    } 

    

    virtual void OverRide() {BasicTest::TearDown();}
};

TEST_F(cubeSkinningTest, checkNumNodes)
{
    ASSERT_EQ(skinnedMesh->n_nodes(), 704);
}

TEST_F(cubeSkinningTest, checkNumElems)
{
    ASSERT_EQ(skinnedMesh->n_elem(), 1404) << "Number of elements does not align with verified mesh";
}

TEST_F(cubeSkinningTest, checkNodeSimilarity)
{
    
    for(int node = 0; node < skinnedMesh->max_node_id(); node++)
    {
        ASSERT_EQ(skinnedMesh->node_ref(node), refSkinnedMesh->node_ref(node));
    }
}

TEST_F(cubeSkinningTest, check2DElems)
{
    for(auto& elem: skinnedMesh->element_ptr_range())
    {
        EXPECT_LT(elem->type(), 7) << "Not all elements are 2D, as would be expected from a skinned mesh";
    }
}