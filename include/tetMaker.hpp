#pragma once

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
#include <unistd.h>
#include <filesystem>
#include "pythonTools.hpp"
#include "visualiseSeedPoints.hpp"


void tetrahedraliseVacuumRegion(std::string filename, std::string outname, Eigen::MatrixXd seedPoints);

Eigen::MatrixXd getSeeds(std::string filename);