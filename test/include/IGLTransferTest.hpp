#pragma once
#include "TransferTest.hpp"

class IGLTransferTest : public TransferTest
{
public:
    
protected:

    IGLTransferTest(std::string meshFile) : TransferTest(meshFile)
    {
    }

    virtual void SetUp() override
    {
        getFilePaths();
        setMesh();
    }

    virtual void setMesh() 
    {   
        TransferTest::setMesh();
        Eigen::MatrixXd V;
        Eigen::MatrixXi F;
        libMeshToIGL(mesh, V, F);
        IGLToLibMesh(refMesh, V, F);
    }
};