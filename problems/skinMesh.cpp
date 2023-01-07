#include<iostream>
#include "libmesh/libmesh.h"
#include "libmesh/enum_io_package.h"
#include "libmesh/enum_elem_type.h"
#include "Tetrahedralisation/tetMaker.hpp"
#include "SurfaceMeshing/surfaceMeshing.hpp"
#include <chrono>
#include "algorithm"


using namespace libMesh;

int main(int argc, char** argv)
{
    try
    {   
        if(argc == 1)
        {
            throw 0;
        }
    } catch(...)
    {
        std::cout << "Error, mesh file argument is required." << std::endl;
        return -1;
    }
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

    // Multimap to store which sides of the elements are boundary sides (i.e. which sides have the null neighbor)
    std::multimap<unsigned int, unsigned int> surfaceFaceMap;

    //Initialise libmesh functions and mpi    
    LibMeshInit init(libmeshArgv.size() - 1, libmeshArgv.data());
    //Create mesh object to store volume mesh
    libMesh::Mesh mesh(init.comm());
    //Create mesh object to store surface mesh
    libMesh::Mesh surfaceMesh(init.comm());


    std::cout << "Reading Mesh" << std::endl;
    //Read volume mesh
    mesh.read(filepath);
    std::cout << "Mesh read successfully" << std::endl;
    std::cout << mesh.spatial_dimension() << std::endl;
    // auto start1 = std::chrono::steady_clock::now();
    getSurface(mesh, surfaceMesh, surfaceFaceMap, true, surfFilepath);
    
    libMesh::Mesh sidesetMesh(init.comm());
    libMesh::Mesh sidesetBoundMesh(init.comm());
    std::set<libMesh::boundary_id_type> bounds;
    bounds.insert(1);
    mesh.get_boundary_info().sync(sidesetMesh);
    sidesetMesh.write("IDK.e");
    getSurface(sidesetMesh, sidesetBoundMesh, surfaceFaceMap, false, "sidesetTest.e");

    // auto end1 = std::chrono::steady_clock::now();
    // std::cout << "Elapsed time in milliseconds: "
    // << std::chrono::duration_cast<std::chrono::milliseconds>(end1 - start1).count()
    // << " ms" << std::endl;
    
    
    



    // std::vector<std::vector<libMesh::dof_id_type>> groups;

    // auto start = std::chrono::steady_clock::now();
    // // groupElems(surfaceMesh, groups);
    // // saveGroupedElems(init, surfaceMesh, groups, "block6_");
    // auto end = std::chrono::steady_clock::now();
    // std::cout << "Elapsed time in milliseconds: "
    // << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
    // << " ms" << std::endl;
    // surfaceMesh.write("surgery.e");

    return 0;
}

