#pragma once

#include "Utils/libmeshConversions.hpp"
#include "libmesh/libmesh.h"
#include "libmesh/mesh.h"
#include <filesystem>
#include <stdlib.h>
#include <string>

// Class to hold both the libmesh and libigl representations of a mesh
class MeshPair {

public:
  // Default constructor
  MeshPair();

  // Most commonly called constructor; Takes in libMesh::Init object to
  // instantiate meshes
  MeshPair(const libMesh::LibMeshInit &init);

  // DESTRUCTOR
  ~MeshPair();

  // Getters for mesh objects
  libMesh::Mesh &libmeshMesh();
  Eigen::MatrixXd &iglVerts();
  Eigen::MatrixXi &iglElems();

  // Methods for creating the mesh representation of the other type
  void createLibmeshAnalogue();
  void createIglAnalogue();

protected:
  // private:
  // Mesh objects
  std::unique_ptr<libMesh::Mesh> libmeshMesh_;
  Eigen::MatrixXd iglVerts_;
  Eigen::MatrixXi iglElems_;
};