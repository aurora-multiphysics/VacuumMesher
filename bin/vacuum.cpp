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
  libMesh::Mesh vacuum_mesh(init.comm());
  mesh.read(flags.infile);

  SurfaceMeshGenerator surfMeshGen(mesh, surface_mesh);
  BoundaryGenerator boundMeshGen(mesh, surface_mesh, boundary_mesh);
  VacuumGenerator vacGenner(mesh, surface_mesh, boundary_mesh, vacuum_mesh, &(surfMeshGen.surface_face_map));

  // Read volume mesh
  surfMeshGen.getSurface();
  boundMeshGen.addBoundary(9, 20, flags.triSettings);
  vacGenner.generateVacuumMesh(flags.tetSettings);
  vacuum_mesh.write("MastU_Test.e");

  return 0;
}
