#pragma once
#include "libmesh/elem.h"
#include "libmesh/mesh.h"
#include "libmesh/libmesh.h"
#include "libmesh/mesh_tools.h"
#include "libmesh/boundary_info.h"
#include "libmesh/enum_io_package.h"
#include "libmesh/enum_elem_type.h"
#include "surfaceMeshing.hpp"
#include <algorithm>
#include "utility"

/**
* findDuplicateVerts() searches the mesh of the original geometry and the vacuum mesh
* for identical nodes. If an identical node is found, then its corresponding ID in the geometry
* mesh is stored in the duplicateNodeIds vector. This vector is then used in a binary search at a 
* later point to avoid adding these points to the vacuum mesh.
*/
void 
findDuplicateVerts(libMesh::Mesh& vacuumMesh, 
                     libMesh::Mesh& geometryMesh, 
                     std::map<int, int>& geomToVacNodes,
                     std::vector<unsigned int>& duplicateNodeIds);

void 
addGeomVerts(libMesh::Mesh& geometryMesh, 
             libMesh::Mesh& vacuumMesh, 
             std::map<int, int>& geomToVacNodes,
             std::vector<unsigned int>& duplicateNodeIds);

void 
getGeometryBoundaries(libMesh::Mesh& geometryMesh, 
                      std::vector<libMesh::dof_id_type>& node_id_list, 
                      std::vector<libMesh::boundary_id_type>& bc_id_list);


void 
getGeometryConnectivity(libMesh::Mesh& geometryMesh, 
                        std::vector<std::vector<unsigned int>> &connectivity);

void 
createFullGeometry(libMesh::Mesh& geometryMesh,
                   libMesh::Mesh& vacuumMesh);

void
remakeBoundaries(libMesh::Mesh& geometryMesh,
                 libMesh::Mesh& vacuumMesh,
                 std::vector<libMesh::dof_id_type> bdr_node_id_list,
                 std::vector<libMesh::boundary_id_type> bc_id_list,
                 std::map<int, int>& geomToVacNodes);

void 
getElemInfo2(libMesh::ElemType& elem_type, libMesh::ElemType& face_type, 
libMesh::Elem* element, int& num_elem_faces, int& num_face_nodes);
                 