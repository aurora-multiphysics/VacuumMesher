#pragma once
#include "libmesh/elem.h"
#include "libmesh/mesh.h"
#include "libmesh/libmesh.h"
#include "libmesh/mesh_tools.h"
#include "libmesh/boundary_info.h"
#include "libmesh/enum_io_package.h"
#include "libmesh/enum_elem_type.h"
#include "surfaceMeshing.hpp"
#include <igl/copyleft/cgal/mesh_boolean.h>
#include <igl/readOFF.h>
#include <igl/writeOFF.h>
#include <igl/per_face_normals.h>
// #include <igl/opengl/glfw/Viewer.h>
#include "pythonTools.hpp"
#include "filesystem"
#include <algorithm>
#include "utility"
#include "libmeshToIgl.hpp"
#include "reverseNormal.hpp"



void genBooleanBound(libMesh::Mesh& boundaryMesh, 
                     libMesh::Mesh& surfaceMesh, 
                     libMesh::Mesh& newMesh);