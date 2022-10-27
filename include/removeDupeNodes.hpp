#pragma once

#include "libmesh/elem.h"
#include "libmesh/mesh.h"
#include "libmesh/libmesh.h"
#include "libmesh/mesh_tools.h"
#include "libmesh/enum_io_package.h"
#include "libmesh/enum_elem_type.h"
#include "libmesh/boundary_info.h"
#include "surfaceMeshing.hpp"
#include "utility"

/**
* A more general purpose util than that found in genFullMesh.hpp. Removes duplicate nodes in
* a mesh by checking all nodes against all other nodes. If a duplicate is found, the node with the lower
* node.id() is kept, and the duplicate node removed. All elements that relied on that duplicate node 
* for connectivity are corrected to use the node that got kept. FOr generating the full mesh (i.e., the geometry
* mesh + the vacuum mesh), the tools found in gen full mesh probably the better choice as they're more efficient,
* but this is a nice to have and can be useful sometimes
*/
void 
removeDupeNodes(libMesh::Mesh& mesh);
