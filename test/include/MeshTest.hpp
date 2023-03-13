#pragma once
#include "BasicTest.hpp"

class MeshTest : public BasicTest
{

public:
    
protected:
    MeshTest(std::string meshfile) : BasicTest(), filename(meshfile)
    {
    }


    virtual void SetUp() override
    {
        setMesh();
    }

    virtual void setMesh() 
    {   
        mesh = std::make_shared<libMesh::Mesh>(init->comm());
        mesh->read(filename);
    }

    //Libmesh Mesh object
    std::shared_ptr<libMesh::Mesh> mesh = nullptr;
    
    //File name of the mesh to be skinned
    std::string filename, filenameNoExt, filepath, refSurfaceFilepath, refBoundaryFilepath, vacuumFilepath;
};