#pragma once

#include "libmesh/elem.h"
#include "libmesh/mesh.h"
#include "libmesh/libmesh.h"
#include "libmesh/enum_io_package.h"
#include "libmesh/enum_elem_type.h"
#include "libmesh/boundary_info.h"
#include "surfaceMeshing.hpp"
#include "utility"


void 
removeDuplicateVerts(libMesh::Mesh& vacuumMesh, 
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