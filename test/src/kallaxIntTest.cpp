
// #include"VacuumTest.hpp"

// class KallaxIntTest : public VacuumTest 
// {
//  protected:
//     KallaxIntTest() : VacuumTest("Kallax.e"){} 

//     virtual void SetUp() override
//     {
//         VacuumTest::SetUp();
//         // Group elements 
//         groupElems(*vacMesh, groups);
//     }

//     virtual void OverRide() {BasicTest::TearDown();}

//     // Vector to store groups of elems
//     std::vector<std::vector<libMesh::dof_id_type>> groups;
// };

// TEST_F(KallaxIntTest, meshReadSuccessful)
// {
//     ASSERT_EQ(mesh->n_nodes(), 2777);
// }

// TEST_F(KallaxIntTest, checkNumOutputPieces)
// {
//     ASSERT_EQ(groups.size(), 4);
// }





