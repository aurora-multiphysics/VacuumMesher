#include "BoundaryGeneration/boundaryGeneration.hpp"
#include "MeshContainer.hpp"
#include "Tetrahedralisation/removeDupeNodes.hpp"
#include "Utils/parseFlags.hpp"
#include <chrono>

int main(int argc, const char **argv) {

  // Parse input flags 
  inputFlags flags(parse_settings(argc, argv));

  // use input flags to set tetgen/triangle switches
  flags.setSwitches();

  // Setup simple argv for use with libMesh init
  std::string appName(argv[0]);
  std::vector<char *> libmeshArgv = {(char *)appName.data()};
  std::cout << flags.tetSettings << std::endl;

  //Initialise libmesh 
  libMesh::LibMeshInit init(libmeshArgv.size() - 1, libmeshArgv.data());

  // Create MeshContainer object to help organise all of the meshes 
  MeshContainer meshes(init, flags.infile.value());

  // If user has not specified the length of the boundary, then it is set here
  if (!flags.boundLen.has_value()) {

// create bounding box around mesh
    auto box = libMesh::MeshTools::create_bounding_box(
        meshes.userMesh().libmeshMesh());

// Use bounding points to determine a suitable boundary length
    flags.boundLen = 1.5 * (box.max() - box.min()).norm();
  }

  std::cout << "Getting surface" << std::endl;

  // Generate the skinned mesh, and store it in meshes.skinnedMesh. The surfaceFaceMap
  // is a multimap that maps from element ID's, to boundary side id's (i.e.
  // the faces of that element that have a null neighbor). This information is later used to create
  // a sideset that represents the boundary of the original mesh.
  getSurface(meshes.userMesh().libmeshMesh(),
             meshes.skinnedMesh().libmeshMesh(), &meshes.surfaceFaceMap());

  // Convert surface mesh to libIGL compatible data structures
  meshes.skinnedMesh().createIglAnalogue();

  // Get seed points. Seeding points are used to 
  Eigen::MatrixXd seed_points =
      getSeeds(meshes.skinnedMesh().libmeshMesh(), 1e-04);

  // Turn surfMesh into boundaryMesh
  addBoundary(meshes.skinnedMesh().libmeshMesh(), meshes.boundaryMesh().libmeshMesh(),
              flags.boundLen.value(),
              flags.boundSubd.value(), flags.triSettings);


  // We got the boundary mesh as a libmesh mesh, but we need its IGL version as well
  meshes.boundaryMesh().createIglAnalogue();

  // If verbose flag is set, output the boundaryMesh
  if (flags.verbose) {
    meshes.boundaryMesh().createLibmeshAnalogue();
    meshes.boundaryMesh().libmeshMesh().write(meshes.boundFilename_);
  }

  // rTree tolerance for combining meshes
  const double tol = 1e-05;

  // Tetrahedralise everything
  tetrahedraliseVacuumRegion(
      meshes.boundaryMesh().iglVerts(), meshes.boundaryMesh().iglElems(),
      meshes.vacuumMesh().libmeshMesh(), seed_points, flags.tetSettings);

  // Combine the meshes 
  combineMeshes(tol, meshes.userMesh().libmeshMesh(),
                meshes.vacuumMesh().libmeshMesh(), meshes.surfaceFaceMap());

  meshes.userMesh().libmeshMesh().write(meshes.vacuumFilename_);
  // // Write the mesh to either the value provided in the input flags, or a
  // default filepath

  return 0;
}