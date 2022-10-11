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
        setMesh();
    }

    virtual void getFilePaths() 
    {
        filenameNoExt = std::filesystem::path(filename).stem().string();
        filepath = "./test/testingMeshes/gold/" + filenameNoExt + ".e"; 
    }

    virtual void setMesh() 
    {   
        getFilePaths();
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
    std::string filename, filenameNoExt, filepath;
};