#include<iostream>
#include <igl/readOFF.h>
#include <igl/writeOBJ.h>
#include <igl/read_triangle_mesh.h>
#include <igl/randperm.h>
#include <igl/orientable_patches.h>
#include <igl/slice.h>
#include <igl/hsv_to_rgb.h>
#include <igl/embree/reorient_facets_raycast.h>
#include <igl/opengl/glfw/Viewer.h>
#include <chrono>

void flipNormals(std::string filename);
