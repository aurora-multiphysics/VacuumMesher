#include"CGALTransferTest.hpp"

class CGALHypTransfer : public CGALTransferTest 
{
 protected:
    CGALHypTransfer() : CGALTransferTest("kallax.e"){} 

    virtual void OverRide() {BasicTest::TearDown();}
};

TEST_F(CGALHypTransfer, checkNumNodes)
{
    ASSERT_EQ(mesh->n_nodes(), refMesh->n_nodes());
}

