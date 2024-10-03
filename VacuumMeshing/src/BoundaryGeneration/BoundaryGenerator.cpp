

#include "BoundaryGeneration/BoundaryGenerator.hpp"
#include "Utils/removeDupeNodes.hpp"

BoundaryGenerator::BoundaryGenerator(libMesh::Mesh &mesh,
                                     libMesh::Mesh &surface_mesh,
                                     libMesh::Mesh &boundary_mesh,
                                     double mesh_merge_tolerance)
    : mesh_(mesh), surface_mesh_(surface_mesh), boundary_mesh_(boundary_mesh),
      mesh_merge_tolerance_(mesh_merge_tolerance), centroid_(3) {}

BoundaryGenerator::~BoundaryGenerator() {}

void BoundaryGenerator::addBoundary(double length, int subdivisions,
                                    std::string tri_flags) {

  Eigen::MatrixXd boundary_vertices;
  Eigen::MatrixXi boundary_elements;

  // Generate boundary mesh
  try {
    genBoundary(boundary_vertices, boundary_elements, length, subdivisions,
                tri_flags);
  } catch (const std::invalid_argument &ex) {
    std::terminate();
  }

  // Turn IGL mesh into libmesh Mesh
  IGLToLibMesh(boundary_mesh_, boundary_vertices, boundary_elements);

  // If unspecified, get merge tolerance
  if (mesh_merge_tolerance_ == 0) {
    setMergeToleranceAuto();
  }

  // Combine IGL mesh with boundary
  combineMeshes(mesh_merge_tolerance_, boundary_mesh_, surface_mesh_);
}

void BoundaryGenerator::addBoundingBoxBoundary(double scaling_x, double scaling_y, double scaling_z, int subdivisions,
                                                std::string tri_flags) {

  Eigen::MatrixXd boundary_vertices;
  Eigen::MatrixXi boundary_elements;

  // Generate boundary mesh
  try {
    genBoundingBoxBoundary(boundary_vertices, boundary_elements, scaling_x, scaling_y, scaling_z, subdivisions, tri_flags);
  } catch (const std::invalid_argument &ex) {
    std::terminate();
  }

  // Turn IGL mesh into libmesh Mesh
  IGLToLibMesh(boundary_mesh_, boundary_vertices, boundary_elements);

  // If unspecified, get merge tolerance
  if (mesh_merge_tolerance_ == 0) {
    setMergeToleranceAuto();
  }

  // Combine IGL mesh with boundary
  combineMeshes(mesh_merge_tolerance_, boundary_mesh_, surface_mesh_);
}

void BoundaryGenerator::genBoundary(Eigen::MatrixXd &boundary_vertices,
                                    Eigen::MatrixXi &boundary_elements,
                                    double length, int subdivisions,
                                    std::string tri_flags) {

  // Check that the boundary is big enough to not overlap with the mesh
  checkBoundary(length);

  // Eigen matrices to store vertices and elements for the boundary of a square (2D edge elements)
  Eigen::MatrixXd square_boundary_verts;
  Eigen::MatrixXi square_boundary_elems;

  // Eigen matrices to store vertices and elements for triangulated square
  Eigen::MatrixXd square_verts;
  Eigen::MatrixXi square_elems;
  Eigen::MatrixXd seeds;

  // Create a square from 1st order edge elements
  genSquare(square_verts, square_elems, length, subdivisions);

  // Triangulate the square that we just created
  // igl::triangle::triangulate(square_boundary_verts, square_boundary_elems, seeds, tri_flags, square_verts,
                            //  square_elems);

  // Rotation matrices to rotate elements
  Eigen::Matrix3d x_rot_base;
  x_rot_base << 1, 0, 0, 0, 0, -1, 0, 1, 0;

  Eigen::Matrix3d y_rot_base;
  y_rot_base << 0, 0, -1, 0, 1, 0, 1, 0, 0;

  // Add a column of zeroes as our "z" coord to make this square exist in 3D
  square_verts.conservativeResize(square_verts.rows(), square_verts.cols() + 1);
  square_verts.col(square_verts.cols() - 1) =
      Eigen::VectorXd::Zero(square_verts.rows());

  // Create vector to hold our rotation matrices, this will make it easier to loop over
  // them later
  std::vector<Eigen::Matrix3d> rot_matrices = {x_rot_base, y_rot_base};

  // Vector to store faces of cubic boundary
  std::vector<Eigen::MatrixXd> new_faces(6, square_verts);

  // Loop over 4 of the 6 squares and rotate them. We only loop
  // over 4 because 2 of them are already orientated correctly for our cubic
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

  for(auto &face: new_faces)
  {
    // Translate vertices so that the centre of the boundary is the centre of the mesh bounding box
    translateMesh(face, centroid_);
  }
  // Eigen::MatrixXi square_elems_2 = square_elems;

  // Combine the 6 square meshes to create the cubic boundary, using the rTree
  //  to avoid having any duplicate nodes
  for (int i = 0; i < 6; i++) {
    combineMeshes(boundary_face_merge_tolerance_, boundary_vertices, boundary_elements,
                  new_faces[i], square_elems);
  }
}

void BoundaryGenerator::genBoundingBoxBoundary(Eigen::MatrixXd &boundary_vertices,
                                               Eigen::MatrixXi &boundary_elements,
                                               double scaling_x, double scaling_y, 
                                               double scaling_z, int subdivisions,
                                               std::string tri_flags) {

  // Get dimensions for the boundary based off of a scaled version of the mesh bounding box
  getBoundaryDimensions(scaling_x, scaling_y, scaling_z);

  // Eigen matrices to store vertices and elements for all faces of bounding box
  Eigen::MatrixXd xy_verts, xz_verts, yz_verts;
  Eigen::MatrixXi xy_elems, xz_elems, yz_elems;
  Eigen::MatrixXd seeds;

  // Get the number of elements along each axis, and store this info
  int x_subdiv, y_subdiv, z_subdiv;
  x_subdiv = subdivisions;
  y_subdiv = (bb_y_dim/bb_x_dim) * x_subdiv;
  z_subdiv = (bb_z_dim/bb_x_dim) * x_subdiv;

  /**
   * For a structure which is very thin in one dimension, the number of subdivisions
   * in this dimension may be evaluated as 0. This is not a valid number of subdivions, so
   * a lower limit of 2 is set. 
   */
  // 
  x_subdiv == 0 ? x_subdiv += 2 : ;
  y_subdiv == 0 ? x_subdiv += 2 : ;
  z_subdiv == 0 ? x_subdiv += 2 : ;

  // Create a square from 1st order edge elements
  genTriangulatedRect(xy_verts, xy_elems, bb_x_dim, bb_y_dim, x_subdiv, y_subdiv, tri_flags);
  genTriangulatedRect(xz_verts, xz_elems, bb_x_dim, bb_z_dim, x_subdiv, z_subdiv, tri_flags);
  genTriangulatedRect(yz_verts, yz_elems, bb_z_dim, bb_y_dim, z_subdiv, y_subdiv, tri_flags);

  // Rotation matrices to rotate elements
  Eigen::Matrix3d x_rot_base;
  x_rot_base << 1, 0, 0, 0, 0, -1, 0, 1, 0;

  Eigen::Matrix3d y_rot_base;
  y_rot_base << 0, 0, -1, 0, 1, 0, 1, 0, 0;

  // Add a column of zeroes as our "z" coord to make this square exist in 3D
  xy_verts.conservativeResize(xy_verts.rows(), xy_verts.cols() + 1);
  xy_verts.col(xy_verts.cols() - 1) =
      Eigen::VectorXd::Zero(xy_verts.rows());

  xz_verts.conservativeResize(xz_verts.rows(), xz_verts.cols() + 1);
  xz_verts.col(xz_verts.cols() - 1) =
      Eigen::VectorXd::Zero(xz_verts.rows());

  yz_verts.conservativeResize(yz_verts.rows(), yz_verts.cols() + 1);
  yz_verts.col(yz_verts.cols() - 1) =
      Eigen::VectorXd::Zero(yz_verts.rows());            

  // Create vector to hold our rotation matrices, this will make it easier to loop over
  // them later
  std::vector<Eigen::Matrix3d> rot_matrices = {x_rot_base, y_rot_base};

  // Vector to store faces of cubic boundary
  std::vector<Eigen::MatrixXd> new_faces = {xy_verts, xy_verts, xz_verts, yz_verts, xz_verts, yz_verts};

  // Loop over 4 of the 6 squares and rotate them. We only loop
  // over 4 because 2 of them are already orientated correctly for our cubic
  // boundary
  for (int i = 2; i < 6; i++) {
    new_faces[i] *= rot_matrices[(i % 2)];
  }

  // Currently every square face of our cubic boundary has its centre at {0, 0,
  // 0}, these need to be translated to make a cube
  translateMesh(new_faces[0], {0, 0, bb_z_dim / 2});
  translateMesh(new_faces[1], {0, 0, -bb_z_dim / 2});
  translateMesh(new_faces[2], {0, bb_y_dim / 2, 0});
  translateMesh(new_faces[3], {bb_x_dim / 2, 0, 0});
  translateMesh(new_faces[4], {0, -bb_y_dim / 2, 0});
  translateMesh(new_faces[5], {-bb_x_dim / 2, 0, 0});


  for(auto &face: new_faces)
  {
    // Translate vertices so that the centre of the boundary is the centre of the mesh bounding box
    translateMesh(face, centroid_);
  }
  // Eigen::MatrixXi square_elems_2 = square_elems;

  std::vector<Eigen::MatrixXi*> elements = {&xy_elems, &xy_elems, &xz_elems, &yz_elems, &xz_elems, &yz_elems};
  // Combine the 6 square meshes to create the cubic boundary, using the rTree
  //  to avoid having any duplicate nodes
  for (int i = 0; i < 6; i++) {
    combineMeshes(boundary_face_merge_tolerance_, boundary_vertices, boundary_elements,
                  new_faces[i], *elements[i]);
  }
}

void BoundaryGenerator::genBoundingBoxBoundary(Eigen::MatrixXd &boundary_vertices,
                                               Eigen::MatrixXi &boundary_elements,
                                               double scaling_factor, int subdivisions,
                                               std::string tri_flags)
{
  genBoundingBoxBoundary(boundary_vertices,
                         boundary_elements,
                         scaling_factor, scaling_factor, scaling_factor, 
                         subdivisions, tri_flags);
}

void BoundaryGenerator::genRect(Eigen::MatrixXd &boundary_verts,
                                Eigen::MatrixXi &boundary_elems, double x_dim, double y_dim,
                                  int x_subdiv, int y_subdiv) {

  // Calculate the number of subdivisions for the longer side of the rectangle
  int total_nodes = (2 * x_subdiv) + (2 * y_subdiv);

  // Resize eigen matrices to correct size
  boundary_verts = Eigen::MatrixXd(total_nodes, 2);
  boundary_elems = Eigen::MatrixXi(total_nodes, 2);
  // Eigen::MatrixXd boundary_verts(total_nodes, 2);
  // Eigen::MatrixXi boundary_elems(total_nodes, 2);

  // edge_verts.

  //
  double increment_x = (double)(x_dim / x_subdiv);
  double increment_y = (double)(y_dim / y_subdiv);

  // Use offset so that the square mesh is symmetric about the x and y axes
  double offset_x = x_dim / 2;
  double offset_y = y_dim / 2;

  // Place all the nodes
  long int node_id = 0;
  for (int i = 0; i <= x_subdiv; i++) {
    boundary_verts.row(node_id) << -offset_x + (i * increment_x), -offset_y;
    node_id++;
  }

  // All the other for loops start at 1 and not 0, this prevents creating a
  // duplicate node
  for (int i = 1; i <= y_subdiv; i++) {
    boundary_verts.row(node_id) << offset_x, (i * increment_y) - offset_y;
    node_id++;
  }

  for (int i = 1; i <= x_subdiv; i++) {
    boundary_verts.row(node_id) << offset_x - (i * increment_x), offset_y;
    node_id++;
  }

  // Last loop has to start at 1 and end at (x_subdiv - 1), 
  // to avoid duplicating the starting node
  for (int i = 1; i <= y_subdiv - 1; i++) {
    boundary_verts.row(node_id) << -offset_x, offset_y - (i * increment_y);
    node_id++;
  }

  // Loop over all elements, and assign them the correct nodes
  for (int elem_id = 0; elem_id < total_nodes - 1; elem_id++) {
    boundary_elems(elem_id, 0) = elem_id;
    boundary_elems(elem_id, 1) = elem_id + 1;
  }

  boundary_elems((total_nodes - 1), 0) = 0;
  boundary_elems((total_nodes - 1), 1) = total_nodes - 1;
}

void BoundaryGenerator::genTriangulatedRect(Eigen::MatrixXd &square_verts,
                                            Eigen::MatrixXi &square_elems, double x_dim, double y_dim,
                                            int x_subdiv, int y_subdiv, std::string tri_flags) {

  // Calculate the number of subdivisions for the longer side of the rectangle
  int total_nodes = (2 * x_subdiv) + (2 * y_subdiv);

  // Resize eigen matrices to correct size
  Eigen::MatrixXd boundary_verts(total_nodes, 2);
  Eigen::MatrixXi boundary_elems(total_nodes, 2);

  genRect(boundary_verts, boundary_elems, x_dim, y_dim, x_subdiv, y_subdiv);
  // //
  // double increment_x = (double)(x_dim / x_subdiv);
  // double increment_y = (double)(y_dim / y_subdiv);

  // // Use offset so that the square mesh is symmetric about the x and y axes
  // double offset_x = x_dim / 2;
  // double offset_y = y_dim / 2;

  // // Place all the nodes
  // long int node_id = 0;
  // for (int i = 0; i <= x_subdiv; i++) {
  //   boundary_verts.row(node_id) << -offset_x + (i * increment_x), -offset_y;
  //   node_id++;
  // }

  // // All the other for loops start at 1 and not 0, this prevents creating a
  // // duplicate node
  // for (int i = 1; i <= y_subdiv; i++) {
  //   boundary_verts.row(node_id) << offset_x, (i * increment_y) - offset_y;
  //   node_id++;
  // }

  // for (int i = 1; i <= x_subdiv; i++) {
  //   boundary_verts.row(node_id) << offset_x - (i * increment_x), offset_y;
  //   node_id++;
  // }

  // // Last loop has to start at 1 and end at (x_subdiv - 1), 
  // // to avoid duplicating the starting node
  // for (int i = 1; i <= y_subdiv - 1; i++) {
  //   boundary_verts.row(node_id) << -offset_x, offset_y - (i * increment_y);
  //   node_id++;
  // }

  // // Loop over all elements, and assign them the correct nodes
  // for (int elem_id = 0; elem_id < total_nodes - 1; elem_id++) {
  //   boundary_elems(elem_id, 0) = elem_id;
  //   boundary_elems(elem_id, 1) = elem_id + 1;
  // }

  // boundary_elems((total_nodes - 1), 0) = 0;
  // boundary_elems((total_nodes - 1), 1) = total_nodes - 1;

  Eigen::MatrixXd seeds;
  igl::triangle::triangulate(boundary_verts, boundary_elems, seeds, 
                             tri_flags, square_verts, square_elems);
}

void BoundaryGenerator::genSquare(Eigen::MatrixXd &edge_verts,
                                  Eigen::MatrixXi &edge_elems, double length,
                                  int subdivisions) {

  genRect(edge_verts, edge_elems, length, length, subdivisions, subdivisions);
  // Resize eigen matrices to correct size
  // verts = Eigen::MatrixXd(4 * subdivisions, 2);
  // elems = Eigen::MatrixXi(4 * subdivisions, 2);

  //
  // double increment = (double)(length / subdivisions);

  // // Use offset so that the square mesh is symmetric about the x and y axes
  // double offset = length / 2;

  // // Place all the nodes
  // long int node_id = 0;
  // for (int i = 0; i <= subdivisions; i++) {
  //   verts.row(node_id) << -offset + (i * increment), -offset;
  //   node_id++;
  // }

  // // All the other for loops start at 1 and not 0, this prevents creating a
  // // duplicate node
  // for (int i = 1; i <= subdivisions; i++) {
  //   verts.row(node_id) << offset, (i * increment) - offset;
  //   node_id++;
  // }

  // for (int i = 1; i <= subdivisions; i++) {
  //   verts.row(node_id) << offset - (i * increment), offset;
  //   node_id++;
  // }

  // // Last loop has to start at 1 and end at (subdivisions - 1), 
  // // to avoid duplicating the starting node
  // for (int i = 1; i <= subdivisions - 1; i++) {
  //   verts.row(node_id) << -offset, offset - (i * increment);
  //   node_id++;
  // }

  // // Loop over all elements, and assign them the correct nodes
  // for (int elem_id = 0; elem_id < (4 * subdivisions) - 1; elem_id++) {
  //   elems(elem_id, 0) = elem_id;
  //   elems(elem_id, 1) = elem_id + 1;
  // }

  // elems(((4 * subdivisions) - 1), 0) = 0;
  // elems(((4 * subdivisions) - 1), 1) = (4 * subdivisions) - 1;
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
                                      std::vector<double> &translationVector) {
  for (int i = 0; i < verts.rows(); i++) {
    for(int j = 0; j < verts.cols(); j++)
    verts.row(i)(j) += translationVector[j];
  }
}

void BoundaryGenerator::translateMesh(Eigen::MatrixXd &verts,
                                      Eigen::Vector3d translationVector) {
  for (int i = 0; i < verts.rows(); i++) {
    verts.row(i) += translationVector;
  }
}

void BoundaryGenerator::checkBoundary(const double &length) {
  libMesh::BoundingBox bbox =
      libMesh::MeshTools::create_nodal_bounding_box(mesh_);
  libMesh::Point point = (bbox.max() - bbox.min());

  // Get centroid of bounding box
  libMesh::Point libmesh_centroid = bbox.min() + (point/2);
  centroid_[0] = libmesh_centroid(0);
  centroid_[1] = libmesh_centroid(1);
  centroid_[2] = libmesh_centroid(2); 

  // Find maximum edge length of bounding box
  double mesh_len = point(0);
  mesh_len < point(1) ? mesh_len : point(1);
  mesh_len < point(2) ? mesh_len : point(2);

  // If maximum edge length of bounding box is larger than the length of the boundary, we probably have a problem
  if (mesh_len > length) {
    throw std::invalid_argument(
        "The cubic boundary will overlap with the mesh. Please provide a "
        "larger boundary length.");
  }
}

void BoundaryGenerator::getBoundaryDimensions(double scaling_x, double scaling_y, double scaling_z) {
  
  // If the scaling factor is less than 1 just throw, the mesh will definitely not work
  if((scaling_x < 1) || (scaling_y < 1) || (scaling_z < 1))
  {
    throw std::invalid_argument("Cannot have a scaling factor < 1 for vacuum boundary, there will be overlaps.");
  }
  
  libMesh::BoundingBox bbox =
      libMesh::MeshTools::create_nodal_bounding_box(mesh_);
  libMesh::Point point = (bbox.max() - bbox.min());

  bb_x_dim = point(0) * scaling_x;
  bb_y_dim = point(1) * scaling_y;
  bb_z_dim = point(2) * scaling_z;

  libMesh::Point libmesh_centroid = bbox.min() + (point/2);
  centroid_[0] = libmesh_centroid(0);
  centroid_[1] = libmesh_centroid(1);
  centroid_[2] = libmesh_centroid(2); 
}

void BoundaryGenerator::setMergeToleranceAuto() {
  VacuumMesher::ClosestPairFinder closestPairBoundary(boundary_mesh_);
  VacuumMesher::ClosestPairFinder closestPairSkin(surface_mesh_);

  mesh_merge_tolerance_ = std::min(closestPairSkin.closestPairMagnitude(),
                                   closestPairBoundary.closestPairMagnitude());
  mesh_merge_tolerance_ /= 10;
}