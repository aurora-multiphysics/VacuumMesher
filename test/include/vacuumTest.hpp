#pragma once
#include "BasicTest.hpp"


class VacuumTest : public BasicTest
{

public:
    

protected:
    VacuumTest(std::string meshFilename) : BasicTest(), filename(meshFilename)
    {
    }


    virtual void SetUp() override
    {
        // Set all the correct file paths and read the starting mesh
        getFilePaths();
        setMesh();

        // Create the surface (aka the skinned) mesh
        getSurface(*mesh, *surfaceMesh);
        // Output skinned mesh
        surfaceMesh->write(path+surfFilename);

        // Get seeding points for tetrahedralisation
        Eigen::MatrixXd seed_points = getSeeds(path+surfFilename);

        // Create skinned mesh with outer boundary, for tetrahedralisation
        createBound(path+surfFilename);
        // Generate vacuum mesh
        tetrahedraliseVacuumRegion(path+surfFilename, tetFilename, seed_points);

        tetFilename = stem + "_tet.e";
        readMesh(vacMesh, path+tetFilename);
    }

    void getFilePaths()
    {
        path = "./test/testingMeshes/";
        filepath = path + filename;
        stem = std::filesystem::path(filename).stem().string();
        surfFilename = stem + "_surf.e";
        boundFilename = stem + "_bound.e";
        tetFilename = stem + "_tet.mesh";
    }

    virtual void setMesh()
    {   
        mesh = std::make_shared<libMesh::Mesh>(init->comm());
        surfaceMesh = std::make_shared<libMesh::Mesh>(init->comm());
        vacMesh = std::make_shared<libMesh::Mesh>(init->comm());
        readMesh(mesh, filepath);
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



    // Libmesh Mesh object
    std::shared_ptr<libMesh::Mesh> mesh = nullptr;

    // Libmesh Mesh object for surface (skinned) mesh
    std::shared_ptr<libMesh::Mesh> surfaceMesh = nullptr;

    // Libmesh object for the vacMesh
    std::shared_ptr<libMesh::Mesh> vacMesh = nullptr;

    //File name of the mesh to be skinned
    std::string filename;
    std::string path, filepath, stem, surfFilename, boundFilename, tetFilename; 
};