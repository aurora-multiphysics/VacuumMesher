#pragma once

#include "Utils/getElemInfo.hpp"
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
#include <CGAL/AABB_halfedge_graph_segment_primitive.h>
#include <CGAL/Surface_mesh.h>


// CGAL typedefs
typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef CGAL::Surface_mesh<K::Point_3> Mesh;
typedef std::vector<K::Point_3> Polyline_type;
typedef std::list< Polyline_type > Polylines;
typedef CGAL::AABB_halfedge_graph_segment_primitive<Mesh> HGSP;
typedef CGAL::AABB_traits<K, HGSP>    AABB_traits;
typedef CGAL::AABB_tree<AABB_traits>  AABB_tree;
typedef K::Point_3 Vertex;
typedef CGAL::Polyhedron_3<K, CGAL::Polyhedron_items_with_id_3> Polyhedron;
typedef CGAL::Nef_polyhedron_3<K> Nef_Polyhedron;

void 
libMeshToIGL(libMesh::Mesh& libmeshMesh, Eigen::MatrixXd& V, Eigen::MatrixXi& F, unsigned int dim=3);

void
IGLToLibMesh(libMesh::Mesh& libmeshMesh, Eigen::MatrixXd& V, Eigen::MatrixXi& F);

void
libmeshToCGAL(libMesh::Mesh& libmeshMesh, Polyhedron & poly);

void
CGALToLibmesh(libMesh::Mesh& libmeshMesh, Polyhedron & poly);

libMesh::ElemType
getElemType(Eigen::MatrixXi& F);

