#include"SkinningTest.hpp"

class kallaxSkinningTest : public SkinningTest 
{
 protected:
    kallaxSkinningTest() : SkinningTest("kallax.e"){} 
    virtual void OverRide() {BasicTest::TearDown();}
};

TEST_F(kallaxSkinningTest, checkNumNodes)
{
    ASSERT_EQ(surfaceMesh->n_nodes(), 2038);
}

TEST_F(kallaxSkinningTest, checkNumElems)
{
    ASSERT_EQ(surfaceMesh->n_elem(), 4056) << "Number of elements does not align with verified mesh";
}

TEST_F(kallaxSkinningTest, checkNodePositions)
{
    refSurfaceMesh->read(refSurfaceFilepath);
    for(int node = 0; node < surfaceMesh->n_nodes(); node++)
    {
        ASSERT_EQ(surfaceMesh->node_ref(node), refSurfaceMesh->node_ref(node));
    }
}

TEST_F(kallaxSkinningTest, check2DElems)
{
    for(auto& elem: surfaceMesh->element_ptr_range())
    {
        EXPECT_LT(elem->type(), 7) << "Not all elements are 2D, as would be expected from a skinned mesh";
    }
}

TEST_F(kallaxSkinningTest, checkNumParts)
{
    std::vector<std::vector<libMesh::dof_id_type>> groups;
    groupElems(*surfaceMesh, groups);
    ASSERT_EQ(groups.size(), 5);
}
