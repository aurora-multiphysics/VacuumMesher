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

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef typename CGAL::Polyhedron_3<K, CGAL::Polyhedron_items_with_id_3> Polyhedron;
typedef typename CGAL::Nef_polyhedron_3<K> Nef_Polyhedron;

void genBooleanBound(libMesh::Mesh& boundaryMesh, 
                     libMesh::Mesh& surfaceMesh, 
                     libMesh::Mesh& newMesh);

Polyhedron 
subtract_volumes_poly(Polyhedron geom, Polyhedron bound);