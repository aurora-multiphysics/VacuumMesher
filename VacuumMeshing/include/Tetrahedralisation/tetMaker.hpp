#pragma once

#include<iostream>
#include "libmesh/mesh.h"
#include <igl/embree/reorient_facets_raycast.h>
#include <igl/copyleft/tetgen/tetrahedralize.h>
#include <igl/read_triangle_mesh.h>
#include <igl/randperm.h>
#include <igl/orientable_patches.h>
#include <igl/slice.h>
#include <igl/hsv_to_rgb.h>
// #include <igl/opengl/glfw/Viewer.h>
#include <igl/writeMESH.h>
#include <igl/writeOFF.h>
#include <igl/decimate.h>
#include <chrono>
#include <unistd.h>
#include <filesystem>
#include "Utils/libmeshConversions.hpp"


/** Method used to generate the tetrehedra between the boundary mesh and the part mesh i.e. generates the vacuum mesh.
  Here the boundaryMesh is the mesh you put into the function, a mesh composed of 2D facets you wish to 
  generate tetrahedra in. The vacuumMesh is where the final tet mesh will be stores. The seedPoints represent
  a set of coordinates that exist within closed surfaces you do NOT want tetrahedra to be generated in, and tetSettings 
  is a string that gets passed to tetgen, the underlying library used for tet generation.*/
void 
tetrahedraliseVacuumRegion(libMesh::Mesh& boundaryMesh, libMesh::Mesh& vacuumMesh, Eigen::MatrixXd& seedPoints, std::string tet_settings);


/** Method used to generate the tetrehedra between the boundary mesh and the part mesh i.e. generates the vacuum mesh.
  Here the boundaryMesh is the mesh you put into the function, a mesh composed of 2D facets you wish to 
  generate tetrahedra in. The vacuumMesh is where the final tet mesh will be stores. The seedPoints represent
  a set of coordinates that exist within closed surfaces you do NOT want tetrahedra to be generated in, and tetSettings 
  is a string that gets passed to tetgen, the underlying library used for tet generation.*/
void 
tetrahedraliseVacuumRegion(libMesh::Mesh& boundaryMesh, libMesh::Mesh& vacuumMesh, std::string tet_settings);


/** Method used to generate the tetrehedra between the boundary mesh and the part mesh i.e. generates the vacuum mesh.
  Here the boundaryMesh is the mesh you put into the function, a mesh composed of 2D facets you wish to 
  generate tetrahedra in. The vacuumMesh is where the final tet mesh will be stores. The seedPoints represent
  a set of coordinates that exist within closed surfaces you do NOT want tetrahedra to be generated in, and tetSettings 
  is a string that gets passed to tetgen, the underlying library used for tet generation.*/
void 
tetrahedraliseVacuumRegion(Eigen::MatrixXd& V, Eigen::MatrixXi& F, libMesh::Mesh& vacuumMesh, Eigen::MatrixXd& seedPoints, std::string tet_settings);


// Get seeds
Eigen::MatrixXd 
getSeeds(libMesh::Mesh mesh, double tol=1e-10);