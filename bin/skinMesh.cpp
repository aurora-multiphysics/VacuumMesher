#include "MeshContainer.hpp"
#include "SurfaceMeshing/surfaceMeshing.hpp"
#include "Tetrahedralisation/tetMaker.hpp"
#include "algorithm"
#include "libmesh/enum_elem_type.h"
#include "libmesh/enum_io_package.h"
#include "libmesh/libmesh.h"
#include <chrono>
#include <iostream>

using namespace libMesh;

int main(int argc, char **argv) {
  try {
    if (argc == 1) {
      throw 0;
    }
  } catch (...) {
    std::cout << "Error, mesh file argument is required." << std::endl;
    return -1;
  }
  std::string appName(argv[0]);
  std::vector<char *> libmeshArgv = {(char *)appName.data()};
  std::string filename(argv[1]);

  // Initialise libmesh functions and mpi
  LibMeshInit init(libmeshArgv.size() - 1, libmeshArgv.data());
  // Mesh container object, that has ownership of the mesh, surfaceMesh, Vacuum
  // Mesh
  MeshContainer meshes(init, filename);

  // Read volume mesh
  auto start1 = std::chrono::steady_clock::now();
  getSurface(meshes.userMesh().libmeshMesh(),
             meshes.skinnedMesh().libmeshMesh(), meshes.surfaceFaceMap(), true,
             meshes.skinFilename_);

  auto end1 = std::chrono::steady_clock::now();
  std::cout << "Elapsed time in milliseconds: "
            << std::chrono::duration_cast<std::chrono::milliseconds>(end1 -
                                                                     start1)
                   .count()
            << " ms" << std::endl;

  return 0;
}
