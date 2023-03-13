#pragma once
#include"MeshTest.hpp"

class BoundaryTest : public MeshTest
{

public:
    

protected:
    BoundaryTest(std::string meshFile) : MeshTest(meshFile)
    {
    }


    virtual void SetUp() override
    {
        // Initialise all meshes, read in starting mesh
        setMesh();

        // 
        getSurf
    }

    virtual void setMesh() override
    {   
        MeshTest::setMesh();
        boundaryMesh = std::make_shared<libMesh::Mesh>(init->comm());
    }

    std::shared_ptr<libMesh::Mesh> boundaryMesh = nullptr;
    //File name of the mesh to be skinned
};