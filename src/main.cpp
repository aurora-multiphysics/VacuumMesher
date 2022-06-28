#include<iostream>
#include "mesh.h"
#include "libmesh/libmesh.h"
#include "libmesh/enum_io_package.h"
#include "libmesh/enum_elem_type.h"
#include "surfaceMeshing.hpp"


using namespace libMesh;

int main(int argc, char** argv)
{
    LibMeshInit init(argc, argv);

    Mesh mesh(init.comm());

    mesh.read("../hypervapotron.e");

    

    std::vector<Elem*> elems;
    for (int i = 0; i < 75613; ++i)
    {
        elems.push_back(mesh.elem_ptr(i));
    }
    std::cout << "Skinning Beginning" << std::endl;
    // getSurface(mesh, elems, init);
}

