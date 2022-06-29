#include<iostream>
#include "mesh.h"
#include "libmesh/libmesh.h"
#include "libmesh/enum_io_package.h"
#include "libmesh/enum_elem_type.h"
#include "surfaceMeshing.hpp"
#include <chrono>


using namespace libMesh;

int main(int argc, char** argv)
{
    LibMeshInit init(argc, argv);

    Mesh mesh(init.comm());

    Mesh surfaceMesh(init.comm());

    mesh.read("../chimeraExo.e");

    std::set<int> elems;
    for (int i = 0; i < 24668779; ++i)
    {
        elems.emplace_hint(elems.end(), i);
    }
    
    mesh.print_info();
    std::cout << "Skinning Beginning" << std::endl;
    // auto start = std::chrono::high_resolution_clock::now();
    getSurface(mesh, surfaceMesh, elems, init);
    const std::string file = "cubeSkinned.e";
    surfaceMesh.write(file);
    surfaceMesh.print_info();

}

