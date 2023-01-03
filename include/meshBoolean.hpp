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
#include <CGAL/AABB_halfedge_graph_segment_primitive.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/Surface_mesh/IO/OFF.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/Nef_polyhedron_3.h>
#include <CGAL/boost/graph/convert_nef_polyhedron_to_polygon_mesh.h>
// #include <igl/opengl/glfw/Viewer.h>
#include "pythonTools.hpp"
#include "filesystem"
#include <algorithm>
#include "utility"
#include "libmeshConversions.hpp"
#include "reverseNormal.hpp"
#include "RTree.hpp"
#include "box.hpp"


void genBooleanBound(libMesh::Mesh& boundaryMesh, 
                     libMesh::Mesh& surfaceMesh);

Polyhedron 
subtract_volumes_poly(Polyhedron& geom, Polyhedron& bound);

void
removeDegenerateTris(libMesh::Mesh& cgalMesh, const double& tol);