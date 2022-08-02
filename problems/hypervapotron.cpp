#include<iostream>
#include "mesh.h"
#include "libmesh/libmesh.h"
#include "libmesh/enum_io_package.h"
#include "libmesh/enum_elem_type.h"
#include "surfaceMeshing.hpp"
#include <igl/readOFF.h>
#include <igl/writeOBJ.h>
#include <chrono>
#include "flipNormals.hpp"


using namespace libMesh;

int main(int argc, char** argv)
{
    LibMeshInit init(argc, argv);

    Mesh mesh(init.comm());

    Mesh surfaceMesh(init.comm());

    mesh.read("../hypervapotron.e");

    std::set<int> elems;
    for (int i = 0; i < 75613; ++i)
    {
        elems.emplace_hint(elems.end(), i);
    }
    
    mesh.print_info();
    std::cout << "Skinning Beginning" << std::endl;

    getSurface(mesh, surfaceMesh, elems);

    //Surface mesh is output for visualisation 
    const std::string file = "hypervapotronSkinned.e";
    surfaceMesh.write(file);
    surfaceMesh.print_info();


    flipNormals("/home/bill/projects/libmesh-skinning/Meshes/CubeinACube.off");

    return 0;
}

