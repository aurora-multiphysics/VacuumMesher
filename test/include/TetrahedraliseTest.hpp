#pragma once
#include "SkinningTest.hpp"


class TetrahedraliseTest : public SkinningTest
{

protected:
    TetrahedraliseTest(std::string meshFile) : SkinningTest(meshFile)
    {
    }

e
    virtual void SetUp() override
    {
        getFilePaths();
        setMesh();
    }

    
    virtual void setMesh() override
    {   
        // MeshTest::setMesh();
        vacuumMesh = std::make_shared<libMesh::Mesh>(init->comm());
        //Set up seeding points for tetrahedralisation
        Eigen::MatrixXd seed_points = getSeeds(surfFilepath);
        tetrahedraliseVacuumRegion(tetrahedraliseFilename, "mesh_vacuum.mesh", seed_points, *vacuumMesh);

        //Set up container for elems you want surfaced 
        getSurface(volumeMesh, *newSurfaceMesh, elems);

        std::cout << cubitSkinFilename << std::endl;
    }

    libMesh::Mesh* vacuumMesh = nullptr; 

    //File name of the corresponding cubit skinned mesh
    std::string tetrahedraliseFilename;
};