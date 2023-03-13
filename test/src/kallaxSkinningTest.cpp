// #include"SkinningTest.hpp"

// class kallaxSkinningTest : public SkinningTest 
// {
//  protected:
//     kallaxSkinningTest() : SkinningTest("kallax.e"){} 
//     virtual void OverRide() {BasicTest::TearDown();}
// };

// TEST_F(kallaxSkinningTest, checkNumNodes)
// {
//     ASSERT_EQ(skinnedMesh->n_nodes(), 2038);
// }

// TEST_F(kallaxSkinningTest, checkNumElems)
// {
//     ASSERT_EQ(skinnedMesh->n_elem(), 4056) << "Number of elements does not align with verified mesh";
// }

// TEST_F(kallaxSkinningTest, checkNodePositions)
// {
//     refskinnedMesh->read(refSurfaceFilepath);
//     for(int node = 0; node < skinnedMesh->n_nodes(); node++)
//     {
//         ASSERT_EQ(skinnedMesh->node_ref(node), refskinnedMesh->node_ref(node));
//     }
// }

// TEST_F(kallaxSkinningTest, check2DElems)
// {
//     for(auto& elem: skinnedMesh->element_ptr_range())
//     {
//         EXPECT_LT(elem->type(), 7) << "Not all elements are 2D, as would be expected from a skinned mesh";
//     }
// }

// TEST_F(kallaxSkinningTest, checkNumParts)
// {
//     std::vector<std::vector<libMesh::dof_id_type>> groups;
//     groupElems(*skinnedMesh, groups);
//     ASSERT_EQ(groups.size(), 5);
// }
