#include "BoundaryGeneration/CoilBoundaryGenerator.hpp"
#include "MeshContainer.hpp"
#include "SurfaceMeshing/SurfaceGenerator.hpp"
#include "Utils/parseFlags.hpp"
#include "VacuumGeneration/VacuumGenerator.hpp"
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

  // Read mesh from user provided flags
  mesh.read(flags.infile.value());

  // Instantiate all mesh generators
  SurfaceMeshGenerator surfMeshGen(mesh, surface_mesh);
  CoilBoundaryGenerator boundMeshGen(mesh, surface_mesh, boundary_mesh,
                                     flags.coil_sideset_one_id,
                                     flags.coil_sideset_two_id);

  // Skin mesh
  surfMeshGen.getSurface();
  // Add boundary to skinned mesh
  boundMeshGen.addBoundary(flags.boundLen.value(), flags.boundSubd.value(),
                           flags.triSettings);
  // Write output mesh
  boundary_mesh.write(flags.outfile.value());

  return 0;
}
