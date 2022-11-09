#pragma once
#include "MeshTest.hpp"
#include "libmeshToIgl.hpp"

class TransferTest : public MeshTest
{

public:
    
protected:
    TransferTest(std::string meshFile) : MeshTest(meshFile)
    {
    }

    virtual void SetUp() override
    {
        getFilePaths();
        setMesh();
    }

    virtual void setMesh() override
    {   
        refMesh = std::make_shared<libMesh::Mesh>(init->comm());
    }

    //Libmesh Mesh object
    std::shared_ptr<libMesh::Mesh> refMesh = nullptr;  
};