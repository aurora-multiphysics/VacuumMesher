#include "generateBoundaries.hpp"

void createBoundary(libMesh::LibMeshInit &init, libMesh::Mesh& surfMesh, libMesh::Mesh& boundaryMesh)
{
    auto box = libMesh::MeshTools::create_bounding_box(surfMesh);
    libMesh::Mesh cubeMesh(init.comm());

    // Scale the bounding box by a scaling factor
    box.min() *= 1.3;
    box.max() *= 1.3;

    // Generate a cube which we will then get the skin of to create boundary mesh
    libMesh::MeshTools::Generation::build_cube(cubeMesh, 5, 5, 5, 
                                              (box.min())(0), (box.max())(0), 
                                              (box.min())(1), (box.max())(1), 
                                              (box.min())(2), (box.max())(2), 
                                              libMesh::TET4);
    // Skin the cubeMesh
    getSurface(cubeMesh, boundaryMesh);

    // Merge meshes

}