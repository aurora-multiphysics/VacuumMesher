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
#include "pythonTools.hpp"
#include "visualiseSeedPoints.hpp"
#include "libmeshConversions.hpp"


// Method used to generate the tetrehedra between the boundary mesh and the part mesh i.e. generates the vacuum mesh
void 
tetrahedraliseVacuumRegion(libMesh::Mesh& boundaryMesh, libMesh::Mesh& vacuumMesh, Eigen::MatrixXd& seedPoints);

void 
tetrahedraliseVacuumRegion(libMesh::Mesh& boundaryMesh, libMesh::Mesh& vacuumMesh);

Eigen::MatrixXd 
getSeeds(libMesh::Mesh mesh);