#include<iostream>
#include "libmesh/libmesh.h"
#include "libmesh/enum_io_package.h"
#include "libmesh/enum_elem_type.h"
#include "Tetrahedralisation/tetMaker.hpp"
#include "SurfaceMeshing/surfaceMeshing.hpp"
#include "Utils/MeshContainer.hpp"
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
    std::string filename(argv[1]);
    
    // Multimap to store which sides of the elements are boundary sides (i.e. which sides have the null neighbor)
    std::multimap<unsigned int, unsigned int> surfaceFaceMap;

    //Initialise libmesh functions and mpi    
    LibMeshInit init(libmeshArgv.size() - 1, libmeshArgv.data());
    // Mesh container object, that has ownership of the mesh, surfaceMesh, Vacuum Mesh
    MeshContainer meshes(init, filename);

    //Read volume mesh
    auto start1 = std::chrono::steady_clock::now();
    getSurface(meshes.userMesh().libmeshMesh(), meshes.skinnedMesh().libmeshMesh(), surfaceFaceMap, true, meshes.skinFilename_);

    auto end1 = std::chrono::steady_clock::now();
    std::cout << "Elapsed time in milliseconds: "
    << std::chrono::duration_cast<std::chrono::milliseconds>(end1 - start1).count()
    << " ms" << std::endl;
    
    
    



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

