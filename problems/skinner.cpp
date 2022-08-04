#include<iostream>
#include "libmesh/libmesh.h"
#include "libmesh/enum_io_package.h"
#include "libmesh/enum_elem_type.h"
#include "tetMaker.hpp"
#include "surfaceMeshing.hpp"
#include <chrono>


using namespace libMesh;

int main(int argc, char** argv)
{
    char** argvLib = "skinner"
    int argcLib = 1;

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
    std::string outputExtension = ".off";
    // std::string token = filename.substr(0, filename.find("."));
    std::string outputFile = path + "Outputs/" + filename.substr(0, filename.find(".")) + "_surface" + outputExtension;
    filename = path + filename;
    
    //Initialise libmesh functions and mpi
    LibMeshInit init(argcLib, argvLib);

    //Create mesh object to store volume mesh
    Mesh mesh(init.comm());

    //Create mesh object to store surface mesh
    Mesh surfaceMesh(init.comm());

    //Read volume mesh
    mesh.read(filename);

    const std::set<int> elems;
    for (int i = 0; i < 24668779; ++i)
    {
        elems.emplace_hint(elems.end(), i);
    }
    
    std::cout << "Skinning Beginning" << std::endl;
    getSurface(mesh, surfaceMesh, elems);

    surfaceMesh.write(file);

    return 0;
}

