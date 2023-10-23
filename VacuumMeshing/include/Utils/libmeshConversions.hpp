#pragma once

#include "Utils/getElemInfo.hpp"
#include "libmesh/boundary_info.h"
#include "libmesh/elem.h"
#include "libmesh/enum_elem_type.h"
#include "libmesh/enum_io_package.h"
#include "libmesh/libmesh.h"
#include "libmesh/mesh.h"
#include "utility"
#include <igl/eigs.h>

void libMeshToIGL(const libMesh::Mesh &libmeshMesh, Eigen::MatrixXd &V,
                  Eigen::MatrixXi &F, unsigned int dim = 3);

void IGLToLibMesh(libMesh::Mesh &libmeshMesh, const Eigen::MatrixXd &V,
                  const Eigen::MatrixXi &F);

libMesh::ElemType getElemType(const Eigen::MatrixXi &F);
