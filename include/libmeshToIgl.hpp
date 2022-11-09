#pragma once

#include "getElemInfo.hpp"
#include "libmesh/elem.h"
#include "libmesh/mesh.h"
#include "libmesh/libmesh.h"
#include "libmesh/enum_io_package.h"
#include "libmesh/enum_elem_type.h"
#include "libmesh/boundary_info.h"
#include <igl/opengl/glfw/Viewer.h>
#include <igl/read_triangle_mesh.h>
#include <igl/copyleft/cgal/mesh_boolean.h>
#include "CGAL/Polyhedron_3.h"
#include "CGAL/Nef_polyhedron_3.h"
#include "CGAL/Polyhedron_incremental_builder_3.h"
#include "CGAL/Polyhedron_items_with_id_3.h" 
#include <igl/copyleft/cgal/assign_scalar.h>


#include "utility"

void 
libMeshToIGL(libMesh::Mesh& libmeshMesh, Eigen::MatrixXd& V, Eigen::MatrixXi& F, unsigned int dim=3);

void
IGLToLibMesh(libMesh::Mesh& libmeshMesh, Eigen::MatrixXd& V, Eigen::MatrixXi& F);

template<typename Polyhedron>
void
libmeshToCGAL(libMesh::Mesh& libmeshMesh, Polyhedron & poly);

template<typename Polyhedron>
void
CGALToLibmesh(libMesh::Mesh& libmeshMesh, Polyhedron & poly);

libMesh::ElemType
getElemType(Eigen::MatrixXi& F);

