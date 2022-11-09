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
        getFilePaths();
        setMesh();
    }

    virtual void getFilePaths() override
    {
        MeshTest::getFilePaths();
        boundaryFilename = filenameNoExt + "_bound.e";
        boundaryPath = "./test/testingMeshes/" + boundaryFilename;
    }

    virtual void setMesh() override
    {   
        createBound(filepath);
        readMesh(mesh, boundaryPath);
    }

    //File name of the mesh to be skinned
    std::string boundaryFilename, boundaryPath;
};