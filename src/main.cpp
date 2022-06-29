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

    mesh.read("../hypervapotron.e");
    // std::set<int> elems = {1, 8};

    // Elem* elem = mesh.elem_ptr(1);
    // Elem* edge = elem->build_edge_ptr(0);
    // std::set< const Elem *>neighbor_set;
    // edge->find_edge_neighbors(neighbor_set);
    // for(auto i: neighbor_set)
    // {
    //     std:: cout << i << std:: endl;
    // }
    std::set<int> elems;
    for (int i = 0; i < 75613; ++i)
    {
        elems.emplace_hint(elems.end(), i);
    }
    

    // std::vector<Elem*> elems;
    // for (int i = 0; i < 75613; ++i)
    // {
    //     elems.push_back(mesh.elem_ptr(i));
    // }
    mesh.print_info();
    // std::cout << "Skinning Beginning" << std::endl;
    // auto start = std::chrono::high_resolution_clock::now();
    // // getSurface(mesh, elems, init);
    // auto stop = std::chrono::high_resolution_clock::now();
    // auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    // std::cout << "Time taken by getSurface = " << duration.count() << " milliseconds" << std::endl;
}

