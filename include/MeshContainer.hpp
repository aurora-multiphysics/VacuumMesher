#pragma once

#include "MeshPair.hpp"
#include "libmesh/libmesh.h"
#include "libmesh/mesh.h"
#include <filesystem>
#include <stdlib.h>
#include <string>

class MeshContainer {

public:
  // Default constructor
  MeshContainer();

  // Most commonly called constructor; Takes in libMesh::Init object to
  // instantiate meshes
  MeshContainer(const libMesh::LibMeshInit &init,
                const std::string meshFilepath);

  // DESTRUCTOR
  ~MeshContainer();

  // Getters for mesh objects
  MeshPair &userMesh();
  MeshPair &skinnedMesh();
  MeshPair &boundaryMesh();
  MeshPair &vacuumMesh();

  // Getter for surfaceFaceMap
  std::multimap<unsigned int, unsigned int> &surfaceFaceMap();

  // Method to set all filenames/paths for meshes
  void setFilenames(const std::string &meshFilepath);

  std::string path_, meshFilename_, skinFilename_, boundFilename_,
      vacuumFilename_;

protected:
  // private:
  // Mesh objects
  MeshPair userMesh_, skinnedMesh_, boundaryMesh_, vacuumMesh_;

  // Map to remember which mesh elements are on the surface. This is
  //  useful when you are creating the final vacuum mesh, and you want to know
  //  which elements are on the boundary between the vacuum and the original
  //  mesh
  std::multimap<unsigned int, unsigned int> surfaceFaceMap_;

  // strings to store filenames for different mesh objects
};