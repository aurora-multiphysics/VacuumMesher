#include<iostream>
#include "libmesh/libmesh.h"
#include "libmesh/enum_io_package.h"
#include "libmesh/enum_elem_type.h"
#include "tetMaker.hpp"
#include "surfaceMeshing.hpp"
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

    std::string filename = argv[1];
    std::string path = "./Meshes/chimBlocks/skinnedBlocks/withBound/tetrahedralised/";
    std::string outputExtension = ".e";
    // std::string token = filename.substr(0, filename.find("."));
    const std::string outputFile = path + "reskinned/" + filename.substr(0, filename.find(".")) + "_surface" + outputExtension;
    filename = path + filename;
    //Initialise libmesh functions and mpi
    for(int i = 1; i<argc; i++)
    {   
        argv[i][0] = 0;
    }
    
    std::cout << argv[0] << std::endl;
    std::cout << argc << std::endl;
    argc -= 1;
    LibMeshInit init(argc, argv);
    //Create mesh object to store volume mesh
    Mesh mesh(init.comm());

    //Create mesh object to store surface mesh
    Mesh surfaceMesh(init.comm());

    std::cout << "Reading Mesh" << std::endl;
    //Read volume mesh
    mesh.read(filename);
    std::cout << "Done Reading Mesh" << std::endl;

    std::vector<int> elems;
    std::cout << "Num Elems: " << mesh.n_elem() << std::endl; 
    elems.reserve(mesh.n_elem());
    for (int i = 0; i < mesh.n_elem(); ++i)
    {
        elems.emplace_back(i);
    }
    
    std::sort(elems.begin(), elems.end());
    
    std::cout << "Skinning Beginning" << std::endl;

    auto start1 = std::chrono::steady_clock::now();
    getSurface(mesh, surfaceMesh, elems);
    auto end1 = std::chrono::steady_clock::now();
    std::cout << "Elapsed time in milliseconds: "
    << std::chrono::duration_cast<std::chrono::milliseconds>(end1 - start1).count()
    << " ms" << std::endl;
    



    // std::vector<std::vector<libMesh::dof_id_type>> groups;

    auto start = std::chrono::steady_clock::now();
    // groupElems(surfaceMesh, groups);
    // saveGroupedElems(init, surfaceMesh, groups);
    auto end = std::chrono::steady_clock::now();
    std::cout << "Elapsed time in milliseconds: "
    << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
    << " ms" << std::endl;
    surfaceMesh.write(outputFile);

    return 0;
}

