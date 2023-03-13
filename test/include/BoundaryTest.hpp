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
        
    }

    virtual void setMesh() override
    {   
        // Use inherited method to initialise initial mesh
        MeshTest::setMesh();
        // Initialise new meshes
        skinnedMesh = std::make_shared<libMesh::Mesh>(init->comm());
        boundaryMesh = std::make_shared<libMesh::Mesh>(init->comm());
        getSurface(mesh, skinnedMesh);
        addBoundary(skinnedMesh, boundaryMesh,)
    }

    std::shared_ptr<libMesh::Mesh> skinnedMesh, boundaryMesh;
    skinnedMesh = nullptr;
    boundaryMesh = nullptr;
    //File name of the mesh to be skinned
};