#pragma once
#include "BasicTest.hpp"
#include "surfaceMeshing.hpp"


class MeshTest : public BasicTest
{

public:
    

protected:
    MeshTest(std::string meshFile) : BasicTest(), meshFilename(meshFile)
    {
    }


    virtual void SetUp() override
    {
    }

    void getFilePaths()
    {
        meshFilename = meshFilename.substr(0, meshFilename.find("."));
        meshFilename = "../Meshes/" + meshFilename + ".e"; 
    }

    virtual void setMesh()
    {   
        std::cout << meshFilename << std::endl;
        mesh = std::make_shared<libMesh::Mesh>(initLibMsh->comm());
        readMesh(mesh, meshFilename);
    }

    virtual void readMesh(std::shared_ptr<libMesh::Mesh> libmeshMesh, std::string filename)
    {
        libmeshMesh->read(filename);
    }

    // virtual void getNodeCoords(std::shared_ptr<libMesh::Mesh> libmeshMesh, std::vector<std::vector<double>& nodes)
    // {
    //     for(int node_num = 0; node_num < libmeshMesh->n_node(); node_num++)
    //     {
    //         libMesh::Node* node = libmeshMesh->node_ptr(node_num);
    //         std::vector<double> coords = {*node[0], *node[1], *node[2]};
    //         nodes.push_back(coords);
    //     }
    // }



    //Libmesh Mesh object
    std::shared_ptr<libMesh::Mesh> mesh = nullptr;
    
    //File name of the mesh to be skinned
    std::string meshFilename;
};