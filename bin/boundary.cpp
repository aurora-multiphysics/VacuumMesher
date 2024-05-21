#include "BoundaryGeneration/BoundaryGenerator.hpp"
#include "MeshContainer.hpp"
#include "SurfaceMeshing/SurfaceGenerator.hpp"
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

  // Read mesh from user provided flags
  mesh.read(flags.infile.value());

  // Instantiate all mesh generators
  SurfaceMeshGenerator surfMeshGen(mesh, surface_mesh);
  BoundaryGenerator boundMeshGen(mesh, surface_mesh, boundary_mesh);

  // Skin mesh
  surfMeshGen.getSurface();

  // Add boundary to skinned mesh
  switch (flags.bound_type)
  {
  case inputFlags::BoundaryType::CUBE:
    boundMeshGen.addBoundary(flags.boundLen.value(), flags.boundSubd.value(),
                             flags.triSettings);
    break;
    
  case inputFlags::BoundaryType::BOUNDING_BOX:
    boundMeshGen.addCoilBoundingBoxBoundary(flags.scale_factor_x, flags.scale_factor_y, flags.scale_factor_z, flags.boundSubd.value(),
                                            flags.triSettings);
    break;
  
  default:
    break;
  }

  // Write output mesh
  boundary_mesh.write(flags.outfile.value());

  return 0;
}
