#pragma once
#include "BasicTest.hpp"

class MeshTest : public BasicTest
{

public:
    
protected:
    MeshTest(std::string meshFile) : BasicTest(), filename(meshFile)
    {
    }


    virtual void SetUp() override
    {
        getFilePaths();
        setMesh();
    }

    virtual void getFilePaths() 
    {
        filenameNoExt = std::filesystem::path(filename).stem().string();
        filepath = "./test/testingMeshes/" + filenameNoExt + "/" + filenameNoExt + ".e"; 
        refSurfaceFilepath = "./test/testingMeshes/" + filenameNoExt + "/" + filenameNoExt + "_surf.e";
    }

    virtual void setMesh() 
    {   
        mesh = std::make_shared<libMesh::Mesh>(init->comm());
        readMesh(mesh, filepath);
    }

    virtual void readMesh(std::shared_ptr<libMesh::Mesh> libmeshMesh, std::string filename) 
    {
        libmeshMesh->read(filename);
    }

    //Libmesh Mesh object
    std::shared_ptr<libMesh::Mesh> mesh = nullptr;
    
    //File name of the mesh to be skinned
    std::string filename, filenameNoExt, filepath, refSurfaceFilepath, refBoundaryFilepath, vacuumFilepath;
};