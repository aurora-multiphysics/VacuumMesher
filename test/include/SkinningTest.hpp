#pragma once
#include "MeshTest.hpp"
#include "surfaceMeshing.hpp"
#include "algorithm"

class SkinningTest : public MeshTest
{

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
        // Perform set mesh from MeshTest
        MeshTest::setMesh();

        // Initialise object to hold surface mesh
        surfaceMesh = std::make_shared<libMesh::Mesh>(init->comm());

        // Generate the surface mesh 
        getSurface(*mesh, *surfaceMesh);
    }

    virtual bool checkAllSurface()
    {
    
    }

    // Libmesh Mesh object to store the skinned mesh
    std::shared_ptr<libMesh::Mesh> surfaceMesh = nullptr;
};