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
    std::cout << filename << std::endl;
    
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


    Eigen::MatrixXd seedPoints(2, 3);
    seedPoints << 
     -0.687560, -0.395, 1.192500,
     0.124686, 0.0, -0.212734;

    // tetrahedraliseVacuumRegion(filename, seedPoints);
}

