#include<iostream>
#include <igl/embree/reorient_facets_raycast.h>
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
#include <igl/opengl/glfw/Viewer.h>
#include <chrono>


void tetrahedraliseVacuumRegion(std::string filename, Eigen::MatrixXd seedPoints);