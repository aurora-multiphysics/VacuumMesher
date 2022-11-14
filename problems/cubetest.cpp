#include<iostream>
#include "libmesh/mesh.h"
#include "libmesh/libmesh.h"
#include "libmesh/node.h"
#include "libmesh/elem.h"
#include "libmesh/enum_io_package.h"
#include "libmesh/enum_elem_type.h"
#include <chrono>
#include <fstream>




using namespace libMesh;

int main(int argc, char** argv)
{
    LibMeshInit init(argc, argv);

    Mesh mesh(init.comm());

    Mesh surfaceMesh(init.comm());

    mesh.read("./Meshes/hex27Cube.e");

    std::ofstream fileout("./libmeshinfo.csv");
    for(int i = 0; i<mesh.n_nodes(); i++)
    {
        Node node = mesh.node_ref(i);
        fileout << (node)(0) << ", " << (node)(1) << ", " << (node)(2) << "\n";
    }

    std::ofstream fileout_elem("./libmesheleminfo.csv");
    for(int i = 0; i<mesh.n_elem(); i++)
    {
        libMesh::Elem& element = mesh.elem_ref(0);
        for(auto& node: element.node_ref_range())
        {
            fileout_elem << (node)(0) << ", " << (node)(1) << ", " << (node)(2) << "\n";
        }
        
    }
}

