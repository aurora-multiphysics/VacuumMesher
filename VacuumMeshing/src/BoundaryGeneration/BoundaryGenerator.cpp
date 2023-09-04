

#include "BoundaryGeneration/boundaryGeneration.hpp"
#include "Tetrahedralisation/removeDupeNodes.hpp"

BoundaryGenerator::BoundaryGenerator(libMesh::Mesh &meshRef,
                                     libMesh::Mesh &surfaceMeshRef,
                                     libMesh::Mesh &boundaryMeshRef)
    : mesh(meshRef), surfaceMesh(surfaceMeshRef),
      boundaryMesh(boundaryMeshRef) {}

void BoundaryGenerator::addBoundary(libMesh::Mesh &skinnedMesh,
                                    libMesh::Mesh &boundaryMesh, double length,
                                    int subdivisions, std::string triSettings) {
  // Data structures to store boundary mesh
  Eigen::MatrixXd tempVerts;
  Eigen::MatrixXi tempElems;
  // Generate boundary mesh
  genBoundary(tempVerts, tempElems, length, subdivisions, triSettings, 1e-04);
  // Turn IGL mesh into libmesh Mesh
  IGLToLibMesh(boundaryMesh, tempVerts, tempElems);
  // Combine IGL mesh with boundary
  combineMeshes(1e-06, boundaryMesh, skinnedMesh);
}

void BoundaryGenerator::genSquare(Eigen::MatrixXd &verts,
                                  Eigen::MatrixXi &elems, double length,
                                  int subdivisions) {
  // Add second order capability!

  //
  double increment = (double)(length / subdivisions);
  // Offset so that the square mesh is symmetric about the x and y axes
  double offset = length / 2;

  long int row_num = 0;
  for (int i = 0; i <= subdivisions; i++) {
    verts.row(row_num) << -offset + (i * increment), -offset;
    row_num++;
  }

  for (int i = 1; i <= subdivisions; i++) {
    verts.row(row_num) << offset, (i * increment) - offset;
    row_num++;
  }

  for (int i = 1; i <= subdivisions; i++) {
    verts.row(row_num) << offset - (i * increment), offset;
    row_num++;
  }

  for (int i = 1; i <= subdivisions; i++) {
    verts.row(row_num) << -offset, offset - (i * increment);
    row_num++;
  }

  for (int i = 0; i < (4 * subdivisions) - 1; i++) {
    elems(i, 0) = i;
    elems(i, 1) = i + 1;
  }
  elems(((4 * subdivisions) - 1), 0) = 0;
  elems(((4 * subdivisions) - 1), 1) = (4 * subdivisions) - 1;
}

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
  // if(elemsOne.cols() != elemsTwo.cols())
  // {
  //     throw std::invalid_argument("can only combine meshes of the same
  //     order"); abort;
  // }
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

void BoundaryGenerator::genBoundary(Eigen::MatrixXd &triVerts,
                                    Eigen::MatrixXi &triElems, double length,
                                    int subdivisions, std::string triSettings,
                                    double tol) {

  Eigen::MatrixXd verts(4 * subdivisions, 2);
  Eigen::MatrixXi elems(4 * subdivisions, 2);

  Eigen::MatrixXd squareVerts(4 * subdivisions, 2);
  Eigen::MatrixXi squareElems(4 * subdivisions, 2);

  // triVerts(4 * subdivisions, 2);
  // triElems(4 * subdivisions, 3);

  Eigen::MatrixXd seeds;

  genSquare(verts, elems, length, subdivisions);

  igl::triangle::triangulate(verts, elems, seeds, triSettings, squareVerts,
                             squareElems);

  // Rotational matrices to rotate elements
  Eigen::Matrix3d x_rot_base;
  x_rot_base << 1, 0, 0, 0, 0, -1, 0, 1, 0;

  Eigen::Matrix3d y_rot_base;
  y_rot_base << 0, 0, -1, 0, 1, 0, 1, 0, 0;

  // Resize squareVerts to work in 3d, as it is currently a 2D mesh,
  //  this is done just by adding a column of zeroes as our "z" coord
  squareVerts.conservativeResize(squareVerts.rows(), squareVerts.cols() + 1);
  squareVerts.col(squareVerts.cols() - 1) =
      Eigen::VectorXd::Zero(squareVerts.rows());

  // Define our rotation matrices
  std::vector<Eigen::Matrix3d> rot_matrices = {x_rot_base, y_rot_base};
  std::vector<Eigen::MatrixXd> newFaces(6, squareVerts);

  squareVerts.col(squareVerts.cols() - 1) =
      Eigen::VectorXd::Constant(squareVerts.rows(), length);
  // squareElems.conservativeResize(squareElems.rows(), squareElems.cols()+1);
  // squareElems.col(squareElems.cols()-1) =
  // Eigen::VectorXi::Zero(squareElems.rows());

  for (int i = 0; i < 4; i++) {
    newFaces[i] *= rot_matrices[(i % 2)];
  }

  translateMesh(newFaces[0], {0, length / 2, 0});
  translateMesh(newFaces[1], {length / 2, 0, 0});
  translateMesh(newFaces[2], {0, -length / 2, 0});
  translateMesh(newFaces[3], {-length / 2, 0, 0});
  translateMesh(newFaces[4], {0, 0, length / 2});
  translateMesh(newFaces[5], {0, 0, -length / 2});

  Eigen::MatrixXi squareElems_2 = squareElems;

  for (int i = 0; i < 6; i++) {
    combineMeshes(tol, triVerts, triElems, newFaces[i], squareElems_2);
  }
}
