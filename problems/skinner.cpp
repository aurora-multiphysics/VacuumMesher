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
            std::cout << "lmao" << std::endl;
            throw 0;
        }
    } catch(...)
    {
        std::cout << "Error, mesh file argument is required." << std::endl;
        return -1;
    }


    std::vector<std::string> arguments = {"skinner"};

    int argcLibmesh = 1;
    char *argsLibmesh[] = {    
        (char*)"skinner",
        NULL
    };

    std::string filename = argv[1];
    std::string path = "../Meshes/";
    std::string outputExtension = ".e";
    // std::string token = filename.substr(0, filename.find("."));
    const std::string outputFile = path + "Outputs/" + filename.substr(0, filename.find(".")) + "_surface" + outputExtension;
    filename = path + filename;
    std::cout << "Libmesh?\n";
    //Initialise libmesh functions and mpi
    LibMeshInit init(argcLibmesh, argsLibmesh);
    std::cout << "Libmesh!\n";

    //Create mesh object to store volume mesh
    Mesh mesh(init.comm());

    //Create mesh object to store surface mesh
    Mesh surfaceMesh(init.comm());

    //Read volume mesh
    mesh.read(filename);

    std::set<int> elems;
    // elems.reserve(mesh.n_elem());
    for (int i = 0; i < mesh.n_elem(); ++i)
    {
        elems.emplace_hint(elems.end(), i);
    }
    // std::sort(elems.begin(), elems.end());
    
    std::cout << "Skinning Beginning" << std::endl;

    auto start = std::chrono::steady_clock::now();

    getSurface(mesh, surfaceMesh, elems);

    auto end = std::chrono::steady_clock::now();

    std::cout << "Elapsed time in milliseconds: "
    << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
    << " ms" << std::endl;

    surfaceMesh.write(outputFile);

    return 0;
}

