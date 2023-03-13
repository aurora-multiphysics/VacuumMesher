#pragma once
#include"MeshTest.hpp"

class SkinningTest : public MeshTest
{

public:
    

protected:
    SkinningTest(std::string meshFile) : MeshTest(meshFile)
    {
    }


    virtual void SetUp() override
    {
        setMesh();
    }

    virtual void setMesh() override
    {   
        // Set up first mesh
        MeshTest::setMesh();

        // 
        skinnedMesh = std::make_shared<libMesh::Mesh>(init->comm());
        refSkinnedMesh = std::make_shared<libMesh::Mesh>(init->comm());

        // 
        getSurface(*mesh, *skinnedMesh);
    }
    std::shared_ptr<libMesh::Mesh> refSkinnedMesh = nullptr;
    std::shared_ptr<libMesh::Mesh> skinnedMesh = nullptr;
    //File name of the mesh to be skinned
};