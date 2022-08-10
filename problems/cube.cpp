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

    std::vector<int> elems;
    elems.reserve(mesh.n_elem());
    for (int i = 0; i < mesh.n_elem(); ++i)
    {
        elems.emplace_back(i);
    }
    std::sort(elems.begin(), elems.end());
    
    mesh.print_info();
    std::cout << "Skinning Beginning" << std::endl;
    // auto start = std::chrono::high_resolution_clock::now();
    getSurface(mesh, surfaceMesh, elems);
    const std::string file = "cubeSkinned.e";
    surfaceMesh.write(file);
    surfaceMesh.print_info();

}

