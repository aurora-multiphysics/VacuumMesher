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
    std::string filename = "hollowSphere_wBound.off";
    // //Initialise libmesh functions and mpi
    // LibMeshInit init(argc, argv);

    // //Create mesh object to store volume mesh
    // Mesh mesh(init.comm());

    // //Create mesh object to store surface mesh
    // Mesh surfaceMesh(init.comm());

    // //Read volume mesh
    // mesh.read("../chimeraExo.e");

    // std::set<int> elems;
    // for (int i = 0; i < 24668779; ++i)
    // {
    //     elems.emplace_hint(elems.end(), i);
    // }
    
    // std::cout << "Skinning Beginning" << std::endl;
    // getSurface(mesh, surfaceMesh, elems);

    // const std::string file = "csutSkinned.e";
    // surfaceMesh.write(file);


    Eigen::MatrixXd seedPoints(1, 3);
    seedPoints << 
     9.5, 0.033055, -0.954985,

    tetrahedraliseVacuumRegion(filename, seedPoints);
}

