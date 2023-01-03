#include<iostream>
#include<iostream>
#include <chrono>
#include "libmesh/libmesh.h"
#include "libmesh/enum_io_package.h"
#include "libmesh/enum_elem_type.h"
#include "Tetrahedralisation/tetMaker.hpp"
#include "SurfaceMeshing/surfaceMeshing.hpp"
#include "algorithm"
#include "Tetrahedralisation/removeDupeNodes.hpp"

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
    std::string appName(argv[0]);

    std::vector<char*> libmeshArgv = {(char*)appName.data()};
    std::string path, filepath, filenameNoExt, surfFilename, boundFilename, tetFilename; 
    std::string surfFilepath, boundFilepath, tetFilepath; 
    std::string filename(argv[1]);

    path = "./Meshes/";
    filepath = path + filename;
    filenameNoExt = filename.substr(0, filename.find("."));

    surfFilename = filenameNoExt + "_surf.e";
    boundFilename = filenameNoExt + "_bound.e";
    tetFilename = filenameNoExt + "_tet.mesh";

    surfFilepath = path + surfFilename; 
    boundFilepath = path + boundFilename;
    tetFilepath = path + tetFilename; 
    
    libMesh::LibMeshInit init(libmeshArgv.size() - 1, libmeshArgv.data());
    //Create mesh object to store original model mesh
    libMesh::Mesh mesh(init.comm());
    //Create mesh object to store surface mesh
    libMesh::Mesh surfMesh(init.comm());
    libMesh::Mesh boundaryMesh(init.comm());
    //Create mesh object to store vacuum mesh
    libMesh::Mesh vacuumMesh(init.comm());
    
    //Read volume mesh
    // mesh.read(filepath);
    surfaceMesh.read(surfFilepath);
    boundaryMesh.read(boundFilepath);
    removeDupeNodes(boundaryMesh);
    std::cout << "Mesh read successfully" << std::endl;
    // 1. Produce Skinned Mesh 
        // Perhaps I need to make sure that the centroid of the shape is set to 0,0,0 at or 
        // BEFORE this point

    // Get the surface mesh
    // getSurface(mesh, surfaceMesh, true, surfFilepath);
    // std::cout << surfFilepath << std::endl;
    

    // 1.5. Enforce consistent normal orientation??

    // 2. Get seed points from skinned Mesh
        // Convert exodus mesh to .off for libIGL
        // Use get seed points 
        // Delete off mesh that was created 
    Eigen::MatrixXd seed_points = getSeeds(surfaceMesh);
    
    // 3. Add bounding volume to skinned mesh
    // createBoundary(init, surfMesh);
    
    // 4. Tetrahedralise
    tetrahedraliseVacuumRegion(surfMesh, vacuumMesh, seed_points);
    vacuumMesh.write("gyroidFull.e");

    return 0;
}
