#pragma once
#include "MeshTest.hpp"
#include "surfaceMeshing.hpp"
#include "libmesh/enum_io_package.h"
#include "libmesh/enum_elem_type.h"

class SkinningTest : public MeshTest
{

protected:
    SkinningTest(std::string meshFile) : MeshTest(meshFile)
    {
    }

    virtual void SetUp() override
    {
        getFilePaths();
        setMesh();
    }

    virtual void setMesh() override
    {   
        // Perform set mesh from MeshTest
        MeshTest::setMesh();

        // Initialise object to hold surface mesh
        surfaceMesh = std::make_shared<libMesh::Mesh>(init->comm());

        // Initialise the reference surfaceMesh we will compare to 
        refSurfaceMesh = std::make_shared<libMesh::Mesh>(init->comm());

        // Generate the surface mesh 
        getSurface(*mesh, *surfaceMesh);
    }

    // Libmesh Mesh object to store the skinned mesh
    std::shared_ptr<libMesh::Mesh> surfaceMesh = nullptr;

    std::shared_ptr<libMesh::Mesh> refSurfaceMesh = nullptr;
};