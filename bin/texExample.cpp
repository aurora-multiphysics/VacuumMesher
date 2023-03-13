#include "BoundaryGeneration/boundaryGeneration.hpp"
#include "MeshContainer.hpp"
#include "Tetrahedralisation/removeDupeNodes.hpp"
#include "Utils/parseFlags.hpp"
#include <chrono>

int main(int argc, const char **argv) {
  inputFlags flags(parse_settings(argc, argv));
  flags.setSwitches();
  // Setup simple argv for use with libMesh init
  std::string appName(argv[0]);
  std::vector<char *> libmeshArgv = {(char *)appName.data()};
  std::cout << flags.tetSettings << std::endl;
  // Set up appropriate file names

  libMesh::LibMeshInit init(libmeshArgv.size() - 1, libmeshArgv.data());
  // Create mesh object to store original model mesh
  MeshContainer meshes(init, flags.infile.value());

  // If user has not specified the length of the boundary, then it is set here
  if (!flags.boundLen.has_value()) {
    // create bounding box around mesh
    auto box = libMesh::MeshTools::create_bounding_box(
        meshes.userMesh().libmeshMesh());

    // Use bounding points to determine a suitable boundary length
    flags.boundLen = 1.5 * (box.max() - box.min()).norm();
  }

  // Multimap to store which sides of the elements are boundary sides (i.e.
  // which sides have the null neighbor)
  getSurface(meshes.userMesh().libmeshMesh(),
             meshes.skinnedMesh().libmeshMesh(), meshes.surfaceFaceMap(),
             flags.verbose);


  // Convert surface mesh to libIGL compatible data stcreateIglAnalogueructures
  meshes.skinnedMesh().createIglAnalogue();

  addBoundary(meshes.skinnedMesh().libmeshMesh(), meshes.boundaryMesh().iglVerts(),
              meshes.boundaryMesh().iglElems(), flags.boundLen.value(),
              flags.boundSubd.value(), flags.triSettings);

  meshes.boundaryMesh().createLibmeshAnalogue();

  meshes.boundaryMesh().libmeshMesh().write("picture.e");

  // Get seed points
  // Eigen::MatrixXd seed_points =
      // getSeeds(meshes.skinnedMesh().libmeshMesh(), 1e-04);

//   // If verbose flag is set, output the boundaryMesh
//   if (flags.verbose) {
//     meshes.boundaryMesh().createLibmeshAnalogue();
//     meshes.boundaryMesh().libmeshMesh().write(meshes.boundFilename_);
//   }
//   int subdivisions = 5;
//   Eigen::MatrixXd squareVerts(4 * subdivisions, 2);
//   Eigen::MatrixXi squareElems(4 * subdivisions, 2);
//   genSquare(squareVerts, squareElems, 15, subdivisions);
//   // std::cout << meshes.skinnedMesh().iglVerts() << std::endl;
//   meshes.skinnedMesh().iglVerts().conservativeResize(meshes.skinnedMesh().iglVerts().rows(), 2);
//   // std::cout << "--------------------------------" << std::endl;
//   // std::cout << "----------AFTER-----------------" << std::endl;
//   // std::cout << "--------------------------------" << std::endl;
//   // std::cout << meshes.skinnedMesh().iglVerts() << std::endl;
//   combineIGLMeshes(meshes.skinnedMesh().iglVerts(), meshes.skinnedMesh().iglElems(), squareVerts, squareElems);
  
//   meshes.skinnedMesh().createLibmeshAnalogue();
//   meshes.skinnedMesh().libmeshMesh().write(meshes.skinFilename_);

//   Eigen::MatrixXd triangleVerts;
//   Eigen::MatrixXi triangleElems;
//   Eigen::MatrixXd H;
//   H.resize(1,2);
//   H << 0,0;
//   igl::triangle::triangulate(meshes.skinnedMesh().iglVerts(), meshes.skinnedMesh().iglElems(), H, "qYYa0.5", triangleVerts, triangleElems);
//   meshes.vacuumMesh().iglVerts() = triangleVerts;
//   meshes.vacuumMesh().iglElems() = triangleElems;
//   meshes.vacuumMesh().createLibmeshAnalogue();
//   meshes.vacuumMesh().libmeshMesh().write("circle_triangle.e");
//   // Write the mesh to either the value provided in the input flags, or a
//   // default filepath

//   return 0;
}