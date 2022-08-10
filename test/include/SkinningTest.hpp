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
        getNodeList();
    }


    void getFilePaths()
    {
        cubitSkinFilename = "../Meshes/cubitSkins/" + meshFilename.substr(0, meshFilename.find(".")) + "_cub_skinned.e"; 
        meshFilename = meshFilename.substr(0, meshFilename.find("."));
        meshFilename = "../Meshes/" + meshFilename + ".e"; 
    }
    
    virtual void setMesh() override
    {   
        getFilePaths();
        MeshTest::setMesh();


        surfaceMesh = std::make_shared<libMesh::Mesh>(init->comm());
        cubitSurfaceMesh = std::make_shared<libMesh::Mesh>(init->comm());
        cubitSurfaceMesh->read(cubitSkinFilename);


        std::vector<int> elems;
        elems.reserve(mesh->n_elem());
        for(int i = 0; i < mesh->n_elem(); ++i)
        {
            elems.emplace_back(i);
        }
        std::sort(elems.begin(), elems.end());
        getSurface(*mesh, *surfaceMesh, elems);

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
    std::string cubitSkinFilename;

    // Libmesh Mesh object to store the skinned mesh
    std::shared_ptr<libMesh::Mesh> surfaceMesh = nullptr;

    // Pointer to libMesh mesh object that contains the cubit skinned mesh 
    std::shared_ptr<libMesh::Mesh> cubitSurfaceMesh = nullptr;

    //Vector to store coordinates of all the nodes on the surface mesh
    std::vector<std::tuple<double, double, double>> surfaceNodes;

    //Vector to store coordinates of all the nodes on the cubit made surface mesh
    std::vector<std::tuple<double, double, double>> cubSurfaceNodes;
};