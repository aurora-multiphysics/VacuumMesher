#pragma once

#include "libmesh/elem.h"
#include "libmesh/mesh.h"
#include "libmesh/libmesh.h"
#include "libmesh/mesh_tools.h"
#include "libmesh/enum_io_package.h"
#include "libmesh/enum_elem_type.h"
#include "libmesh/boundary_info.h"
#include "surfaceMeshing.hpp"
#include "generateBoundaries.hpp"
#include "tetMaker.hpp"
#include "RTree.hpp"
#include "utility"

/**
* A more general purpose util than that found in genFullMesh.hpp. Removes duplicate nodes in
* a mesh by checking all nodes against all other nodes. If a duplicate is found, the node with the lower
* node.id() is kept, and the duplicate node removed. All elements that relied on that duplicate node 
* for connectivity are corrected to use the node that got kept. FOr generating the full mesh (i.e., the geometry
* mesh + the vacuum mesh), the tools found in gen full mesh probably the better choice as they're more efficient,
* but this is a nice to have and can be useful sometimes
*/

struct Box
{
    Box();
    Box(std::array<double, 3> node,
        double tol,
        unsigned int node_id)
    {
        centre = node;

        min = {node[0] - tol,
               node[1] - tol,
               node[2] - tol};

        max = {node[0] + tol,
               node[1] + tol,
               node[2] + tol};

        this->node_id = node_id;
    }

    public:
        std::array<double,3> centre; /// the midpoint of the box
        std::array<double,3> min;    /// lowest coordinates of the box
        std::array<double,3> max;    /// largest coorinates of the box
        unsigned int node_id;
};

void 
insertNode(RTree<int, double, 3, float> &rtree, Box& node_box);

void
createTree(RTree<int, double, 3, float> &rtree, libMesh::Mesh& mesh, double& tol);

bool
searchTree(RTree<int, double, 3, float> &rtree, libMesh::Mesh& mesh, libMesh::Node& node);

void 
combineMesh(double& tol, libMesh::Mesh& surfMesh, 
            libMesh::Mesh& vacMesh, 
            std::multimap<unsigned int, unsigned int> surfaceFaceMap);



