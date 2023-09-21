#include "BoundaryGeneration/CoilBoundaryGenerator.hpp"
#include "MeshContainer.hpp"
#include "SurfaceMeshing/SurfaceGenerator.hpp"
#include "VacuumGeneration/VacuumGenerator.hpp"
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

  // Instantiate all our mesh objects
  libMesh::Mesh mesh(init.comm());
  libMesh::Mesh surface_mesh(init.comm());
  libMesh::Mesh boundary_mesh(init.comm());
  mesh.read("../hive_coil.e");

  SurfaceMeshGenerator surfMeshGen(mesh, surface_mesh);
  CoilBoundaryGenerator boundMeshGen(mesh, surface_mesh, boundary_mesh);
  
  // Read volume mesh
  surfMeshGen.getSurface();
  boundMeshGen.addBoundary(0.2, 20, flags.triSettings);

  return 0;
}
