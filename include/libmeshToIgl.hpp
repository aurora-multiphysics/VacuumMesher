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


#include "utility"

void 
libMeshToIGL(libMesh::Mesh& libmeshMesh, Eigen::MatrixXd& V, Eigen::MatrixXi& F, unsigned int dim=3);

void
IGLToLibMesh(libMesh::Mesh& libmeshMesh, Eigen::MatrixXd& V, Eigen::MatrixXi& F);

libMesh::ElemType
getElemType(Eigen::MatrixXi& F);