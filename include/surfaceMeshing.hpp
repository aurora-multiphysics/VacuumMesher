#pragma once

#include "libmesh/elem.h"
#include "libmesh/mesh.h"
#include "libmesh/libmesh.h"
#include "libmesh/mesh_tools.h"
#include "libmesh/boundary_info.h"
#include "libmesh/enum_io_package.h"
#include "libmesh/enum_elem_type.h"
#include "getElemInfo.hpp"


// using namespace libMesh;

void getSurface(libMesh::Mesh& mesh, libMesh::Mesh& surfaceMesh, std::multimap<unsigned int, unsigned int>& surfaceFaceMap, bool writeMesh=false, std::string outputFilename = "surface_mesh.e");

void getSurface(libMesh::Mesh& mesh, libMesh::Mesh& surfaceMesh, bool writeMesh=false, std::string outputFilename = "surface_mesh.e");

void getSurface(libMesh::Mesh& mesh, libMesh::Mesh& surfaceMesh, std::vector<int>& elSet, bool writeMesh = false, std::string outputFilename = "surface_mesh.e");

int getNumSurfaceElems();

void isElementSurface(libMesh::Elem* element, std::vector<int>& elSet, std::vector<int>& surfaceFaces);

void isElementSurface(libMesh::Elem* element, std::vector<int>& surfaceFaces);

void groupElems(libMesh::Mesh mesh, std::vector<std::vector<libMesh::dof_id_type>>& groups);

void saveGroupedElems(libMesh::LibMeshInit& init, libMesh::Mesh& surfaceMesh, 
                      std::vector<std::vector<libMesh::dof_id_type>>& groups,
                      std::string componentFilename);