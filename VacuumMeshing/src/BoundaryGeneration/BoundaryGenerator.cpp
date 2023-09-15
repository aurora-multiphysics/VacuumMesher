

#include "BoundaryGeneration/BoundaryGenerator.hpp"
#include "Utils/removeDupeNodes.hpp"

BoundaryGenerator::BoundaryGenerator(libMesh::Mesh &meshRef,
                                     libMesh::Mesh &surfaceMeshRef,
                                     libMesh::Mesh &boundaryMeshRef)
    : mesh(meshRef), surfaceMesh(surfaceMeshRef),
      boundaryMesh(boundaryMeshRef) {}

BoundaryGenerator::~BoundaryGenerator() {}

void BoundaryGenerator::addBoundary(double length, int subdivisions,
                                    std::string triSettings) {

  Eigen::MatrixXd boundary_verts;
  Eigen::MatrixXi boundary_elems;
  // Generate boundary mesh
  genBoundary(boundary_verts, boundary_elems, length, subdivisions, triSettings,
              1e-04);
  // Turn IGL mesh into libmesh Mesh
  IGLToLibMesh(boundaryMesh, boundary_verts, boundary_elems);
  // Combine IGL mesh with boundary
  combineMeshes(1e-06, boundaryMesh, surfaceMesh);
  boundaryMesh.write("bound.e");
}

void BoundaryGenerator::genBoundary(Eigen::MatrixXd &triVerts,
                                    Eigen::MatrixXi &triElems, double length,
                                    int subdivisions, std::string triSettings,
                                    double tol) {

  Eigen::MatrixXd verts(4 * subdivisions, 2);
  Eigen::MatrixXi elems(4 * subdivisions, 2);

  Eigen::MatrixXd squareVerts(4 * subdivisions, 2);
  Eigen::MatrixXi squareElems(4 * subdivisions, 2);

  Eigen::MatrixXd seeds;

  // Create a square from 1st order edge elements
  genSquare(verts, elems, length, subdivisions);

  // Triangulate the square that we just created
  igl::triangle::triangulate(verts, elems, seeds, triSettings, squareVerts,
                             squareElems);

  // Rotation matrices to rotate elements
  Eigen::Matrix3d x_rot_base;
  x_rot_base << 1, 0, 0, 0, 0, -1, 0, 1, 0;

  Eigen::Matrix3d y_rot_base;
  y_rot_base << 0, 0, -1, 0, 1, 0, 1, 0, 0;

  // Add a column of zeroes as our "z" coord to make this square exist in 3D
  squareVerts.conservativeResize(squareVerts.rows(), squareVerts.cols() + 1);
  squareVerts.col(squareVerts.cols() - 1) =
      Eigen::VectorXd::Zero(squareVerts.rows());

  // Vector to hold our rotation matrices, this will make it easier to loop over
  // them later
  std::vector<Eigen::Matrix3d> rot_matrices = {x_rot_base, y_rot_base};

  // Vector
  std::vector<Eigen::MatrixXd> new_faces(6, squareVerts);

  squareVerts.col(squareVerts.cols() - 1) =
      Eigen::VectorXd::Constant(squareVerts.rows(), length);

  // Loop over 4 of the 6 squares and rotate them. The reason for only looping
  // over 4 is that
  //  2 of them are already orientated correctly for our cubic boundary
  for (int i = 0; i < 4; i++) {
    new_faces[i] *= rot_matrices[(i % 2)];
  }

  // Currently every square face of our cubic boundary has its centre at {0, 0,
  // 0},
  //  these need to be translated to make a cube
  translateMesh(new_faces[0], {0, length / 2, 0});
  translateMesh(new_faces[1], {length / 2, 0, 0});
  translateMesh(new_faces[2], {0, -length / 2, 0});
  translateMesh(new_faces[3], {-length / 2, 0, 0});
  translateMesh(new_faces[4], {0, 0, length / 2});
  translateMesh(new_faces[5], {0, 0, -length / 2});

  Eigen::MatrixXi squareElems_2 = squareElems;

  // Combine the 6 square meshes to create the cubic boundary, using the rTree
  //  to avoid having any duplicate nodes
  for (int i = 0; i < 6; i++) {
    combineMeshes(tol, triVerts, triElems, new_faces[i], squareElems_2);
  }
}

void BoundaryGenerator::genSquare(Eigen::MatrixXd &verts,
                                  Eigen::MatrixXi &elems, double length,
                                  int subdivisions) {

  //
  double increment = (double)(length / subdivisions);

  // Use offset so that the square mesh is symmetric about the x and y axes
  double offset = length / 2;

  // Place all the nodes
  long int node_id = 0;
  for (int i = 0; i <= subdivisions; i++) {
    verts.row(node_id) << -offset + (i * increment), -offset;
    node_id++;
  }

  // All the other for loops start at 1 and not 0, this prevents creating a
  // duplicate node
  for (int i = 1; i <= subdivisions; i++) {
    verts.row(node_id) << offset, (i * increment) - offset;
    node_id++;
  }

  for (int i = 1; i <= subdivisions; i++) {
    verts.row(node_id) << offset - (i * increment), offset;
    node_id++;
  }

  // Last loop has to start at 1 and end at (subdivisions - 1), to avoid the
  // starting node
  for (int i = 1; i <= subdivisions - 1; i++) {
    std::cout << i << std::endl;
    verts.row(node_id) << -offset, offset - (i * increment);
    node_id++;
  }

  // Loop over all elements, and assign them the correct nodes
  for (int elem_id = 0; elem_id < (4 * subdivisions) - 1; elem_id++) {
    elems(elem_id, 0) = elem_id;
    elems(elem_id, 1) = elem_id + 1;
  }
  elems(((4 * subdivisions) - 1), 0) = 0;
  elems(((4 * subdivisions) - 1), 1) = (4 * subdivisions) - 1;
}

// Method for combining IGL meshes that we know DO NOT OVERLAP. If you are not
// sure don't use this,
//  use one of the rTree methods
void BoundaryGenerator::combineIGLMeshes(Eigen::MatrixXd &vertsOne,
                                         Eigen::MatrixXi &elemsOne,
                                         Eigen::MatrixXd &vertsTwo,
                                         Eigen::MatrixXi &elemsTwo) {
  if (vertsOne.cols() != vertsTwo.cols()) {
    std::cout << vertsOne.cols() << " VS. " << vertsTwo.cols() << std::endl;
    throw std::invalid_argument(
        "combineIGLMeshes can only combined meshes of the same dimension");
    abort;
  }

  // Store rows in vertsOne before we resize it
  int initial_node_rows = vertsOne.rows();
  int initial_elem_rows = elemsOne.rows();
  // Resize vertsOne to store info from vertsTwo
  vertsOne.conservativeResize(vertsOne.rows() + vertsTwo.rows(),
                              vertsOne.cols());
  elemsOne.conservativeResize(elemsOne.rows() + elemsTwo.rows(),
                              elemsOne.cols());

  for (int i = 0; i < vertsTwo.rows(); i++) {
    vertsOne.row(initial_node_rows + i) = vertsTwo.row(i);
  }

  for (int i = 0; i < elemsTwo.rows(); i++) {
    elemsOne.row(initial_elem_rows + i) =
        elemsTwo.row(i) +
        Eigen::VectorXi::Constant(elemsOne.cols(), initial_node_rows);
  }
}

void BoundaryGenerator::translateMesh(Eigen::MatrixXd &verts,
                                      Eigen::Vector3d translationVector) {
  for (int i = 0; i < verts.rows(); i++) {
    verts.row(i) += translationVector;
  }
}