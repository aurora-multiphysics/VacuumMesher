#include "MeshContainer.hpp"

// Default constructor
MeshContainer::MeshContainer() {
  // Doesn't do alot...
}

MeshContainer::~MeshContainer() {}

// Constructor that should be used. Initialises mesh objects using init, and
// takes in
//  a filepath to the mesh the user wants to use
MeshContainer::MeshContainer(const libMesh::LibMeshInit &init,
                             const std::string meshFilepath)
    : userMesh_(init), skinnedMesh_(init), boundaryMesh_(init),
      vacuumMesh_(init) {
  setFilenames(meshFilepath);
  std::cout << meshFilepath << std::endl;
  // Read mesh from file user has specified
  std::cout << "Reading Mesh" << std::endl;
  userMesh_.libmeshMesh().read(meshFilepath);
  std::cout << "Mesh read successfully" << std::endl;
}

void MeshContainer::setFilenames(const std::string &meshFilepath) {
  std::filesystem::path fullPath(meshFilepath);
  path_ = fullPath.parent_path().string();
  meshFilename_ =
      path_ + "/" + fullPath.stem().string() + fullPath.extension().string();
  skinFilename_ = path_ + "/" + fullPath.stem().string() + "_surf" +
                  fullPath.extension().string();
  boundFilename_ = path_ + "/" + fullPath.stem().string() + "_bound" +
                   fullPath.extension().string();
  vacuumFilename_ = path_ + "/" + fullPath.stem().string() + "_vac" +
                    fullPath.extension().string();
}

MeshPair &MeshContainer::userMesh() { return userMesh_; }

MeshPair &MeshContainer::skinnedMesh() { return skinnedMesh_; }

MeshPair &MeshContainer::boundaryMesh() { return boundaryMesh_; }

MeshPair &MeshContainer::vacuumMesh() { return vacuumMesh_; }

std::multimap<unsigned int, unsigned int> &MeshContainer::surfaceFaceMap() {
  return surfaceFaceMap_;
}