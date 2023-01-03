#pragma once 

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
#include "Utils/getElemInfo.hpp"
#include "BoundaryGeneration/meshBoolean.hpp"


// Method to create boundary around the surface mesh. The space between the surface mesh and
//  the boundary mesh are what becomes the vacuum mesh
void createBoundary(libMesh::LibMeshInit &init, 
                    libMesh::Mesh& surfMesh,
                    double scaleFactor);

// Very similar method, but makes sure the boundary is made in such a way so that
//  one surface is coplanar with the coil in/out faces
void createCoilBoundary(libMesh::LibMeshInit &init, 
                        libMesh::Mesh& surfMesh, 
                        double scaleFactor);                    