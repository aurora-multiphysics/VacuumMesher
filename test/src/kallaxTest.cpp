
#include"vacuumTest.hpp"

class KallaxTest : public VacuumTest 
{
 protected:
    KallaxTest() : VacuumTest("Kallax.e"){} 

    virtual void SetUp() 
    {
        VacuumTest::SetUp();
        // Group elements 
        groupElems(*vacMesh, groups);
    }

    virtual void OverRide() {BasicTest::TearDown();}

    // Vector to store groups of elems
    std::vector<std::vector<libMesh::dof_id_type>> groups;
};

TEST_F(KallaxTest, meshReadSuccessful)
{
    ASSERT_EQ(mesh->n_nodes(), 2777);
}

TEST_F(KallaxTest, checkNumOutputPieces)
{
    ASSERT_EQ(groups.size(), 4);
}





