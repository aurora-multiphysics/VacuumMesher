#pragma once

#include "libmesh/elem.h"
#include "libmesh/mesh.h"
#include "libmesh/libmesh.h"
#include "libmesh/mesh_tools.h"
#include "libmesh/enum_io_package.h"
#include "libmesh/enum_elem_type.h"
#include "libmesh/boundary_info.h"
#include "SurfaceMeshing/surfaceMeshing.hpp"
#include "BoundaryGeneration/generateBoundaries.hpp"
#include "Tetrahedralisation/tetMaker.hpp"
#include "Utils/RTree.hpp"
#include "utility"
#include "Utils/box.hpp"

// typedef for RTree
typedef RTree<int, double, 3, float> nodeTree;
// Method for inserting individual nodes into rTree. Instantiates their Box,
//  and then uses the rTree insert methods to add it.
void 
insertNode(nodeTree &rtree, Box& node_box);

// Function for creating the initial tree. This method adds all the nodes from a libmesh mesh
//  into the rTree. Then, when adding more nodes to this mesh, we can check the rTree to see if they
//   already exist 
void
createTree(nodeTree &rtree, libMesh::Mesh& mesh, const double& tol);

// Function for searching the rTree. Takes in a libMesh node and checks if it already exists
//  in the tree. Note the argument, "mesh". This refers to the mesh that already has it's
//  nodes in the rTree, and was likely passed into createTree. Here it is passed in so we can know
//  the number of nodes it has, so that if we do find a novel node to add, we can know what ID to give it to
//  ensure that it is higher than that which already exists in the mesh. We also keep a map "id_map" to map 
//  from old node id's to new node id's for the nodes being added, as well as ones that stay the same
bool
searchTree(nodeTree &rtree, 
           const double& tol, 
           std::map<unsigned int, unsigned int>& id_map, 
           libMesh::Mesh& meshOne, 
           libMesh::Node& node);

// Calls the functions that have been defined to create the combined mesh
void 
combineMeshes(const double& tol, 
              libMesh::Mesh& meshTwo, 
              libMesh::Mesh& meshOne, 
              std::multimap<unsigned int, unsigned int> surfaceFaceMap);

void 
combineMeshes(const double& tol,
              libMesh::Mesh& meshOne,
              libMesh::Mesh& meshTwo);

void
combineMeshes(const double& tol,
                Eigen::MatrixXd& V1,
                Eigen::MatrixXi& F1,
                Eigen::MatrixXd& V2,
                Eigen::MatrixXi& F2);

