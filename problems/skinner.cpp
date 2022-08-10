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
    auto start = std::chrono::steady_clock::now();
    try
    {   
        if(argc == 1)
        {
            std::cout << "lmao" << std::endl;
            throw 0;
        }
    } catch(...)
    {
        std::cout << "Error, mesh file argument is required." << std::endl;
        return -1;
    }

    std::string filename = argv[1];
    std::string path = "../Meshes/";
    std::string outputExtension = ".e";
    // std::string token = filename.substr(0, filename.find("."));
    const std::string outputFile = path + "Outputs/" + filename.substr(0, filename.find(".")) + "_surface" + outputExtension;
    filename = path + filename;
    //Initialise libmesh functions and mpi
    for(int i = 1; i<argc; i++)
    {   
        argv[i][0] = 0;
    }
    
    std::cout << argv[0] << std::endl;
    argc = 1;
    LibMeshInit init(argc, argv);
    //Create mesh object to store volume mesh
    Mesh mesh(init.comm());

    //Create mesh object to store surface mesh
    Mesh surfaceMesh(init.comm());

    //Read volume mesh
    mesh.read(filename);

    std::vector<int> elems;
    elems.reserve(mesh.n_elem());
    for (int i = 0; i < mesh.n_elem(); ++i)
    {
        elems.emplace_back(i);
    }
    std::sort(elems.begin(), elems.end());
    
    std::cout << "Skinning Beginning" << std::endl;


    getSurface(mesh, surfaceMesh, elems);

    auto end = std::chrono::steady_clock::now();

    std::cout << "Elapsed time in milliseconds: "
    << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
    << " ms" << std::endl;

    surfaceMesh.write(outputFile);

    return 0;
}

