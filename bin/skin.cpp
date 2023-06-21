#include "BoundaryGeneration/boundaryGeneration.hpp"
#include "MeshContainer.hpp"
#include "SurfaceMeshing/surfaceMeshing.hpp"
#include "Tetrahedralisation/removeDupeNodes.hpp"
#include "Utils/parseFlags.hpp"
#include <chrono>

// using namespace libMesh;

int main(int argc, const char **argv) {
  inputFlags flags(parse_settings(argc, argv));
  flags.setSwitches();

  // Setup simple argv for use with libMesh init
  std::string appName(argv[0]);
  std::vector<char *> libmeshArgv = {(char *)appName.data()};

  // Initialise libmesh functions and mpi
  libMesh::LibMeshInit init(libmeshArgv.size() - 1, libmeshArgv.data());
  // Mesh container object, that has ownership of the mesh, surfaceMesh, Vacuum
  MeshContainer meshes(init, flags.infile.value());

  // Read volume mesh
  getSurface(meshes.userMesh().libmeshMesh(), meshes.skinnedMesh().libmeshMesh(), &(meshes.surfaceFaceMap()), true);
  return 0;
}
