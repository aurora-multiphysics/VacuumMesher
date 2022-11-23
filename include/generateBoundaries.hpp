#pragma once 

#include "genFullMesh.hpp"
#include "libmesh/elem.h"
#include "libmesh/mesh.h"
#include "libmesh/libmesh.h"
#include "libmesh/enum_io_package.h"
#include "libmesh/enum_elem_type.h"
#include "libmesh/boundary_info.h"
#include "libmesh/mesh_generation.h"
#include <igl/copyleft/tetgen/tetrahedralize.h>
#include <igl/read_triangle_mesh.h>
#include <igl/randperm.h>
#include <igl/orientable_patches.h>
#include <igl/slice.h>
#include <igl/hsv_to_rgb.h>
#include <igl/opengl/glfw/Viewer.h>
#include <igl/writeMESH.h>
#include <igl/writeOFF.h>
#include <igl/decimate.h>
#include "utility"
#include "getElemInfo.hpp"

void createBoundary(libMesh::LibMeshInit &init, libMesh::Mesh& surfMesh, libMesh::Mesh& boundaryMesh);