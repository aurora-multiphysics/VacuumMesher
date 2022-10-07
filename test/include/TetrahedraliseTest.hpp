#pragma once
#include "MeshTest.hpp"
#include "surfaceMeshing.hpp"
#include "tetMaker.hpp"

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


    virtual void getFilePaths() override
    {
        std::string token = meshFilename.substr(0, meshFilename.find("."));
        tetrahedraliseFilename = meshFilename;
        meshFilename = "../Meshes/Surfaces/" + meshFilename; 
    }
    
    virtual void setMesh() override
    {   
        getFilePaths();
        // MeshTest::setMesh();
        surfaceMesh = std::make_shared<libMesh::Mesh>(init->comm());
        newSurfaceMesh = std::make_shared<libMesh::Mesh>(init->comm());
        surfaceMesh->read(meshFilename);

        //Set up seeding points for tetrahedralisation
        Eigen::
        tetrahedraliseVacuumRegion(tetrahedraliseFilename, )

        //Set up container for elems you want surfaced 
        getSurface(volumeMesh, *newSurfaceMesh, elems);

        std::cout << cubitSkinFilename << std::endl;
    }

    virtual void getNodeList()
    {
        
        for(int node_num = 0; node_num < cubitSurfaceMesh->n_nodes(); node_num++)
        {
            libMesh::Node* cubNode = surfaceMesh->node_ptr(node_num);
            libMesh::Node* surNode = cubitSurfaceMesh->node_ptr(node_num);
       
            std::tuple<double, double, double> cubCoord = {(*cubNode)(0), (*cubNode)(1), (*cubNode)(2)};
            std::tuple<double, double, double> surCoord = {(*surNode)(0), (*surNode)(1), (*surNode)(2)};

            cubSurfaceNodes.push_back(cubCoord);
            surfaceNodes.push_back(surCoord);
        }

        sort(cubSurfaceNodes.begin(), cubSurfaceNodes.end());
        sort(surfaceNodes.begin(), surfaceNodes.end());
    }


    //File name of the corresponding cubit skinned mesh
    std::string tetrahedraliseFilename;

    //
    // std:

    // Libmesh Mesh object to store the skinned mesh
    std::shared_ptr<libMesh::Mesh> surfaceMesh = nullptr;

    // Pointer to libMesh mesh object that contains the cubit skinned mesh 
    std::shared_ptr<libMesh::Mesh> newSurfaceMesh = nullptr;

    //Vector to store coordinates of all the nodes on the surface mesh
    std::vector<std::tuple<double, double, double>> surfaceNodes;

    //Vector to store coordinates of all the nodes on the cubit made surface mesh
    std::vector<std::tuple<double, double, double>> cubSurfaceNodes;
};