

#include "BoundaryGeneration/BoundaryGenerator.hpp"
#include "Utils/removeDupeNodes.hpp"

BoundaryGenerator::BoundaryGenerator(libMesh::Mesh &mesh,
                                     libMesh::Mesh &surface_mesh,
                                     libMesh::Mesh &boundary_mesh,
                                     double mesh_merge_tolerance)
    : mesh_(mesh), surface_mesh_(surface_mesh), boundary_mesh_(boundary_mesh),
      mesh_merge_tolerance_(mesh_merge_tolerance) {}

BoundaryGenerator::~BoundaryGenerator() {}

void BoundaryGenerator::addBoundary(double length, int subdivisions,
                                    std::string tri_flags) {

  Eigen::MatrixXd boundary_verts;
  Eigen::MatrixXi boundary_elems;

  // Generate boundary mesh
  try {
    genBoundary(boundary_verts, boundary_elems, length, subdivisions,
                tri_flags);
  } catch (const std::invalid_argument &ex) {
    std::terminate();
  }

  // Turn IGL mesh into libmesh Mesh
  IGLToLibMesh(boundary_mesh_, boundary_verts, boundary_elems);

  // If unspecified, get merge tolerance
  if (mesh_merge_tolerance_ == 0) {
    setMergeToleranceAuto();
  }

  // Combine IGL mesh with boundary
  combineMeshes(mesh_merge_tolerance_, boundary_mesh_, surface_mesh_);
}

void BoundaryGenerator::genBoundary(Eigen::MatrixXd &tri_vertices,
                                    Eigen::MatrixXi &tri_elements,
                                    double length, int subdivisions,
                                    std::string tri_flags) {

  checkBoundary(length);

  // Initialise matrices to be of correct size
  // Eigen::MatrixXd verts(4 * subdivisions, 2);
  // Eigen::MatrixXi elems(4 * subdivisions, 2);
  Eigen::MatrixXd verts;
  Eigen::MatrixXi elems;

  // Matrices initialised for triangulated square
  Eigen::MatrixXd square_verts;
  Eigen::MatrixXi square_elems;
  Eigen::MatrixXd seeds;

  // Create a square from 1st order edge elements
  genSquare(verts, elems, length, subdivisions);

  // Triangulate the square that we just created
  igl::triangle::triangulate(verts, elems, seeds, tri_flags, square_verts,
                             square_elems);

  // Rotation matrices to rotate elements
  Eigen::Matrix3d x_rot_base;
  x_rot_base << 1, 0, 0, 0, 0, -1, 0, 1, 0;

  Eigen::Matrix3d y_rot_base;
  y_rot_base << 0, 0, -1, 0, 1, 0, 1, 0, 0;

  // Add a column of zeroes as our "z" coord to make this square exist in 3D
  square_verts.conservativeResize(square_verts.rows(), square_verts.cols() + 1);
  square_verts.col(square_verts.cols() - 1) =
      Eigen::VectorXd::Zero(square_verts.rows());

  // Vector to hold our rotation matrices, this will make it easier to loop over
  // them later
  std::vector<Eigen::Matrix3d> rot_matrices = {x_rot_base, y_rot_base};

  // Vector to hold faces of cubic boundary
  std::vector<Eigen::MatrixXd> new_faces(6, square_verts);

  // Loop over 4 of the 6 squares and rotate them. The reason for only looping
  // over 4 is that 2 of them are already orientated correctly for our cubic
  // boundary
  for (int i = 0; i < 4; i++) {
    new_faces[i] *= rot_matrices[(i % 2)];
  }

  // Currently every square face of our cubic boundary has its centre at {0, 0,
  // 0}, these need to be translated to make a cube
  translateMesh(new_faces[0], {0, length / 2, 0});
  translateMesh(new_faces[1], {length / 2, 0, 0});
  translateMesh(new_faces[2], {0, -length / 2, 0});
  translateMesh(new_faces[3], {-length / 2, 0, 0});
  translateMesh(new_faces[4], {0, 0, length / 2});
  translateMesh(new_faces[5], {0, 0, -length / 2});

  Eigen::MatrixXi square_elems_2 = square_elems;

  // Combine the 6 square meshes to create the cubic boundary, using the rTree
  //  to avoid having any duplicate nodes
  for (int i = 0; i < 6; i++) {
    combineMeshes(boundary_face_merge_tolerance_, tri_vertices, tri_elements,
                  new_faces[i], square_elems_2);
  }
}

void BoundaryGenerator::genSquare(Eigen::MatrixXd &verts,
                                  Eigen::MatrixXi &elems, double length,
                                  int subdivisions) {

  // Resize eigen matrices to correct size
  verts = Eigen::MatrixXd(4 * subdivisions, 2);
  elems = Eigen::MatrixXi(4 * subdivisions, 2);

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
// sure don't use this, use one of the rTree methods
void BoundaryGenerator::combineIGLMeshes(Eigen::MatrixXd &vertices_one,
                                         Eigen::MatrixXi &elements_one,
                                         Eigen::MatrixXd &vertices_two,
                                         Eigen::MatrixXi &elements_two) {

  assert(vertices_one.cols() == vertices_two.cols() &&
         "combineIGLMeshes can only combine meshes of the same dimension");

  // Store rows in vertices_one before we resize it
  int initial_node_rows = vertices_one.rows();
  int initial_elem_rows = elements_one.rows();
  // Resize vertices_one to store info from vertices_two
  vertices_one.conservativeResize(vertices_one.rows() + vertices_two.rows(),
                                  vertices_one.cols());
  elements_one.conservativeResize(elements_one.rows() + elements_two.rows(),
                                  elements_one.cols());

  for (int i = 0; i < vertices_two.rows(); i++) {
    vertices_one.row(initial_node_rows + i) = vertices_two.row(i);
  }

  for (int i = 0; i < elements_two.rows(); i++) {
    elements_one.row(initial_elem_rows + i) =
        elements_two.row(i) +
        Eigen::VectorXi::Constant(elements_one.cols(), initial_node_rows);
  }
}

void BoundaryGenerator::translateMesh(Eigen::MatrixXd &verts,
                                      Eigen::Vector3d translationVector) {
  for (int i = 0; i < verts.rows(); i++) {
    verts.row(i) += translationVector;
  }
}

void BoundaryGenerator::checkBoundary(const double &length) const {
  libMesh::BoundingBox bbox =
      libMesh::MeshTools::create_nodal_bounding_box(mesh_);
  libMesh::Point point = (bbox.max() - bbox.min());
  // Find maximum edge length of bounding box
  double mesh_len = point(0);
  mesh_len < point(1) ? mesh_len : point(1);
  mesh_len < point(2) ? mesh_len : point(2);

  if (mesh_len > length) {
    throw std::invalid_argument(
        "The cubic boundary will overlap with the mesh. Please provide a "
        "larger boundary length.");
  }
}

void BoundaryGenerator::setMergeToleranceAuto() {
  VacuumMesher::ClosestPairFinder closestPairBoundary(boundary_mesh_);
  VacuumMesher::ClosestPairFinder closestPairSkin(surface_mesh_);

  mesh_merge_tolerance_ = std::min(closestPairSkin.closestPairMagnitude(),
                                   closestPairBoundary.closestPairMagnitude());
  mesh_merge_tolerance_ /= 10;
}