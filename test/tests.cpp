#include<iostream>
#include <gtest/gtest.h>

#include "libmesh/mesh.h"
#include "libmesh/libmesh.h"
#include "libmesh/enum_io_package.h"
#include "libmesh/enum_elem_type.h"




using namespace libMesh;

int main(int argc, char** argv)
{
    LibMeshInit init(argc, argv);
    std::cout << "o" << std::endl;
    Mesh mesh(init.comm());

    mesh.read("../hypervapotron.e");

    mesh.print_info();

}