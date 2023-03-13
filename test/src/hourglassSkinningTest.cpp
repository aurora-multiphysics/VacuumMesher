// #include"SkinningTest.hpp"

// class hourglassSkinningTest : public SkinningTest 
// {
//  protected:
//     hourglassSkinningTest() : SkinningTest("hourglass.e"){} 

//     virtual void OverRide() {BasicTest::TearDown();}
// };

// TEST_F(hourglassSkinningTest, checkNumNodes)
// {
//     ASSERT_EQ(surfaceMesh->n_nodes(), 2127) << "Number of nodes is not equal to verified mesh";
// }

// TEST_F(hourglassSkinningTest, checkNumElems)
// {
//     ASSERT_EQ(surfaceMesh->n_elem(), 4246) << "Number of elements is not equal to verified mesh";
// }

// TEST_F(hourglassSkinningTest, checkNodePositions)
// {
//     refSurfaceMesh->read(refSurfaceFilepath);
//     for(int node = 0; node < surfaceMesh->n_nodes(); node++)
//     {
//         ASSERT_EQ(surfaceMesh->node_ref(node), refSurfaceMesh->node_ref(node));
//     }
// }

// TEST_F(hourglassSkinningTest, check2DElems)
// {
//     for(auto& elem: surfaceMesh->element_ptr_range())
//     {
//         EXPECT_LT(elem->type(), 7) << "Not all elements are 2D, as would be expected from a skinned mesh";;
//     }
// }
