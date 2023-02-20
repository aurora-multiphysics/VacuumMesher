#include "MeshPair.hpp"

// Default constructor
MeshPair::MeshPair()
{
    // Doesn't do alot...
}

MeshPair::~MeshPair()
{

}

// Constructor that should be used. Initialises mesh objects using init, and takes in
//  a filepath to the mesh the user wants to use 
MeshPair::MeshPair(const libMesh::LibMeshInit& init)
{
    // Instantiate mesh objects
    libmeshMesh_ = std::make_unique<libMesh::Mesh>(init.comm());
}

libMesh::Mesh& MeshPair::libmeshMesh() 
{
    return *libmeshMesh_;
}

Eigen::MatrixXd& MeshPair::iglVerts() 
{
    return iglVerts_;
}

Eigen::MatrixXi& MeshPair::iglElems() 
{
    return iglElems_;
}

void MeshPair::createLibmeshAnalogue()
{
    // Call conversion method from "libmeshConversions.hpp"
    IGLToLibMesh(libmeshMesh(), iglVerts_, iglElems_);
}

void MeshPair::createIglAnalogue()
{
    // Call conversion method from "libmeshConversions.hpp"
    libMeshToIGL(libmeshMesh(), iglVerts_, iglElems_);
}