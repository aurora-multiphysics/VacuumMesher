#include<iostream>
#include<iostream>
#include <chrono>
#include "libmesh/libmesh.h"
#include "libmesh/enum_io_package.h"
#include "libmesh/enum_elem_type.h"
#include "tetMaker.hpp"
#include "surfaceMeshing.hpp"
#include "algorithm"
#include"visualiseSeedPoints.hpp"
#include "pythonTools.hpp"

int main(int argc, char** argv)
{   
    // 0. Set up libmesh environment and meshes
    // try
    // {   
    //     if(argc == 1)
    //     {
    //         throw 0;
    //     }
    // } catch(...)
    // {
    //     std::cout << "Error, mesh file argument is required." << std::endl;
    //     return -1;
    // }
    std::cout << "Huh" << std::endl;
    std::string appName(argv[0]);
    std::cout << "Checkpoint 1" << std::endl;
    std::vector<char*> libmeshArgv = {(char*)appName.data()};
    std::string path, filenameNoExt, surfFilename, filepath; 
    std::string filename(argv[1]);
    std::cout << "Checkpoint 2" << std::endl;
    path = "./Meshes/";
    filepath = path + filename;
    filenameNoExt = filename.substr(0, filename.find("."));

    surfFilename = filenameNoExt + "_surf.e";
      
    libMesh::LibMeshInit init(libmeshArgv.size() - 1, libmeshArgv.data());
    std::cout << "Checkpoint 3" << std::endl;
    //Create mesh object to store original model mesh
    libMesh::Mesh mesh(init.comm());

    //Create mesh object to store surface mesh
    libMesh::Mesh surfaceMesh(init.comm());
    
    //Read volume mesh
    
    mesh.read(filepath);
    std::cout << "Mesh read successfully" << std::endl;

    // 1. Produce Skinned Mesh 
        // Perhaps I need to make sure that the centroid of the shape is set to 0,0,0 at or 
        // BEFORE this point
    // Get the surface mesh
    getSurface(mesh, surfaceMesh);
    // Write out the surface mesh
    surfaceMesh.write(path+surfFilename);
    std::cout << "Checkpoint 4" << std::endl;


    // 1.5. Enforce consistent normal orientation??

    // 2. Get seed points from skinned Mesh
        // Convert exodus mesh to .off for libIGL
        // Use get seed points 
        // Delete off mesh that was created 
    Eigen::MatrixXd seed_points = getSeeds(path+surfFilename);
    
    // 3. Add bounding volume to skinned mesh
        // Should be able to choose shape type and size 
        // Should check that the bounding area is larger than the bounding box of the shape 
    createBound(filepath);
    
    // 4. Tetrahedralise

    // tetrahedraliseVacuumRegion(filename, outputFile, seed_points);

    // 5. Output
        // Should the output be one big exodu

    // visualiseSeedPoints(filename, H);
    // tetrahedraliseVacuumRegion(filename, outputFile, seed_points);
    return 0;
}
