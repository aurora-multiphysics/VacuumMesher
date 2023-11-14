#include "BoundaryGeneration/CoilBoundaryGenerator.hpp"
#include "SurfaceMeshing/SurfaceGenerator.hpp"
#include "Utils/removeDupeNodes.hpp"

// CoilBoundaryGenerator::CoilBoundaryGenerator(){}

CoilBoundaryGenerator::CoilBoundaryGenerator(libMesh::Mesh &mesh,
                                             libMesh::Mesh &surface_mesh,
                                             libMesh::Mesh &boundary_mesh,
                                             const int sideset_one_id,
                                             const int sideset_two_id,
                                             double merge_tolerance)
    : BoundaryGenerator(mesh, surface_mesh, boundary_mesh, merge_tolerance),
      coil_sideset_one_id(sideset_one_id), coil_sideset_two_id(sideset_two_id) {
}

CoilBoundaryGenerator::~CoilBoundaryGenerator() {}

void CoilBoundaryGenerator::addBoundary(const double length,
                                        const int subdivisions,
                                        const std::string tri_flags) {

  // generate the boundary and store it in boundary_mesh_
  generateCoilBoundary(length, subdivisions, tri_flags);

  if (mesh_merge_tolerance_ == 0) {
    setMergeToleranceAuto();
    std::cout << "Mesh merge tolerance used: " << mesh_merge_tolerance_
              << std::endl;
  }
  // Combine the boundary mesh with the surface mesh to create a mesh ready for
  // tetrahedralisation
  combineMeshes(mesh_merge_tolerance_, boundary_mesh_, surface_mesh_);
}

void CoilBoundaryGenerator::generateCoilBoundary(const double length,
                                                 const int subdivisions,
                                                 const std::string tri_flags) {

  // Libmesh mesh_ that stores only sideset data
  libMesh::Mesh sideset_mesh(mesh_.comm());
  libMesh::Mesh sideset_mesh_skinned(mesh_.comm());

  // Generate sideset mesh_ (uses default argument for sideset names)
  genSidesetMesh(mesh_, sideset_mesh);

  // Use a SurfaceMeshGenerator to "skin" the mesh of the sidesets. This results
  // in a mesh of edge elements
  SurfaceMeshGenerator sidesetSkinner(sideset_mesh, sideset_mesh_skinned);
  sidesetSkinner.getSurface();

  // Create Eigen 3x3 matrix to store 3 points from the co-planar sidesets, that
  // will be used to define the plane that they sit on
  Eigen::Matrix3d plane_points;

  // Populate plane points matrix with the nodes belonging to an element on
  // the sidesets
  for (int row_id = 0; row_id < 3; row_id++) {
    auto &node = sideset_mesh.elem_ref(0).node_ref(row_id);
    plane_points.row(row_id) << node(0), node(1), node(2);
  }

  // Eigen 3x3 Matrix that will store the basis vectors of our transformed
  // cartesian system
  Eigen::Matrix3d basis_matrix;

  // Define new origin as a point on the coil sideset(s), this will be fairly
  // central
  Eigen::Vector3d origin = plane_points.row(0);

  // Generate basis matrix from 3 points that define a plane
  getBasisMatrix(basis_matrix, plane_points);

  // Eigen objects to store
  Eigen::MatrixXd sideset_vertices, cube_vertices;
  Eigen::MatrixXi sideset_element_verts, cube_element_verts;

  // Convert the sideset mesh_ to
  libMeshToIGL(sideset_mesh_skinned, sideset_vertices, sideset_element_verts);

  // Do a change of basis on the mesh
  changeMeshBasis(sideset_vertices, origin, basis_matrix);

  // Define seed points matrix
  Eigen::MatrixXd seed_points(2, 3);

  // Get the seed points of the coplanar coil boundaries
  getCoplanarSeedPoints(mesh_, seed_points);

  // Transform seed points into new coordinate system
  for (int i = 0; i < seed_points.rows(); i++) {
    Eigen::Vector3d point(seed_points.row(i));
    seed_points.row(i) = calculateLocalCoords(point, origin, basis_matrix);
  }

  // Generates the face of the boundary that is coplanar with the coil sidesets
  // (the special boundary face)
  generateCoilFaceBound(sideset_vertices, sideset_element_verts, seed_points,
                        cube_vertices, cube_element_verts, length, subdivisions,
                        tri_flags);

  // Generates the 5 other faces of the cubic boundary
  genRemainingFiveBoundaryFaces(cube_vertices, cube_element_verts, length,
                                subdivisions, tri_flags);

  // Change back to original basis
  changeMeshBasis(cube_vertices, {0, 0, 0}, Eigen::Matrix3d::Identity(), origin,
                  basis_matrix);

  // Convert our IGL mesh back to libMesh
  IGLToLibMesh(boundary_mesh_, cube_vertices, cube_element_verts);
}

bool CoilBoundaryGenerator::getBasisMatrix(
    Eigen::Matrix3d &basis_matrix, const Eigen::Matrix3d &plane_points) {

  // Basis vectors X Y and Z
  Eigen::Vector3d X, Y, Z;

  // Vectors between the 3 points that will define our plane
  Eigen::Vector3d AB = (plane_points.row(1) - plane_points.row(0));
  Eigen::Vector3d AC = (plane_points.row(2) - plane_points.row(0));

  // Normalise basis vectors so that their magnitude is one, relative to
  // original basis
  X = AB.normalized();
  Z = (AC.cross(AB)).normalized();
  Y = (X.cross(Z)).normalized();
  // Add our basis vectors to 'basis_matrix' matrix
  basis_matrix.col(0) << X;
  basis_matrix.col(1) << Y;
  basis_matrix.col(2) << Z;

  return true;
}

Eigen::Vector3d CoilBoundaryGenerator::calculateLocalCoords(
    Eigen::Vector3d &point, Eigen::Vector3d new_origin,
    Eigen::Matrix3d new_basis, Eigen::Vector3d old_origin,
    Eigen::Matrix3d old_basis) {
  Eigen::Vector3d local_coords =
      new_basis.inverse() * (old_origin - new_origin + (old_basis * point));
  return local_coords;
}

void CoilBoundaryGenerator::generateCoilFaceBound(
    const Eigen::MatrixXd &verts, const Eigen::MatrixXi &elems,
    const Eigen::MatrixXd &holes, Eigen::MatrixXd &tri_vertices,
    Eigen::MatrixXi &tri_elems, double length, int subdivisions,
    std::string tri_flags) {
  //
  Eigen::MatrixXd holes2D = holes.block(0, 0, 2, 2);
  Eigen::MatrixXd verts2D = verts.block(0, 0, verts.rows(), 2);
  Eigen::MatrixXi elems2D = elems;

  // Generate a border around the skinned sideset, which is then used to define
  // space for triangulation
  genSidesetBounds(verts2D, elems2D, length, subdivisions);

  // Perform Delauny triangulation
  igl::triangle::triangulate(verts2D, elems2D, holes2D, tri_flags, tri_vertices,
                             tri_elems);

  // Resize triangle vertices matrix to have 3 cols instead of 2 (2D -> 3D), set
  // all z coords to 0
  tri_vertices.conservativeResize(tri_vertices.rows(), tri_vertices.cols() + 1);
  tri_vertices.col(tri_vertices.cols() - 1) =
      Eigen::VectorXd::Zero(tri_vertices.rows());
}

void CoilBoundaryGenerator::generateCoilFaceBound(
    libMesh::Mesh &mesh, libMesh::Mesh &output_mesh,
    libMesh::Mesh &remaining_boundary, Eigen::MatrixXd &holes,
    std::string &tri_args) {

  //
  Eigen::MatrixXd verts, newVerts;
  Eigen::MatrixXi elems, newElems;
  //
  genSidesetBounds(mesh, remaining_boundary);
  //
  libMeshToIGL(mesh, verts, elems, 2);
  //
  igl::triangle::triangulate(verts, elems, holes, tri_args, newVerts, newElems);
  IGLToLibMesh(output_mesh, newVerts, newElems);
  //
  combineMeshes(boundary_face_merge_tolerance_, output_mesh,
                remaining_boundary);
}

void CoilBoundaryGenerator::genSidesetMesh(libMesh::Mesh &mesh,
                                           libMesh::Mesh &sideset_mesh) {

  // Create set to store ids of sidesets that will be included in mesh
  std::set<libMesh::boundary_id_type> ids;
  // Insert the sideset id's into a set to get a mesh containing both of them
  ids.insert(coil_sideset_one_id);
  ids.insert(coil_sideset_two_id);

  // Get sideset boundary
  mesh.get_boundary_info().sync(ids, sideset_mesh);
}

void CoilBoundaryGenerator::genSidesetBounds(Eigen::MatrixXd &verts,
                                             Eigen::MatrixXi &elems,
                                             const double length,
                                             const int subdivisions) {
  Eigen::MatrixXd square_verts((4 * subdivisions), 2);
  Eigen::MatrixXi square_elems((4 * subdivisions), 2);

  genSquare(square_verts, square_elems, length, subdivisions);

  combineIGLMeshes(verts, elems, square_verts, square_elems);
}

void CoilBoundaryGenerator::genSidesetBounds(
    libMesh::Mesh &sideset_mesh,
    libMesh::Mesh &boundary_without_coilside_face) {

  //
  std::unordered_map<u_int, libMesh::dof_id_type> id_map;
  libMesh::Mesh skinned_square(sideset_mesh.comm());

  // Here skin
  SurfaceMeshGenerator boundary_skin_generator(boundary_without_coilside_face,
                                               skinned_square);
  boundary_skin_generator.getSurface();

  // Starting node ID for points
  libMesh::dof_id_type starting_node = sideset_mesh.max_node_id() + 1;
  libMesh::dof_id_type starting_elem = sideset_mesh.max_elem_id() + 1;

  // Build mesh of 2D edge elements, including the skinned sideset and a square
  // boundary
  for (auto &node : skinned_square.node_ptr_range()) {
    double pnt[2];
    for (u_int i = 0; i < 2; i++) {
      pnt[i] = (*node)(i);
    }
    libMesh::Point xyz(pnt[0], pnt[1]);
    id_map[node->id()] = starting_node;
    sideset_mesh.add_point(xyz, starting_node);
    starting_node++;
  }

  for (auto &elem : skinned_square.element_ptr_range()) {
    libMesh::Elem *newElem = libMesh::Elem::build(elem->type()).release();
    for (int j = 0; j < elem->n_nodes(); j++) {
      // std::cout << id_map[elem->node_id(j)] << std::endl;
      newElem->set_node(j) = sideset_mesh.node_ptr(id_map[elem->node_id(j)]);
    }
    newElem->set_id(starting_elem++);
    sideset_mesh.add_elem(newElem);
  }

  //
  sideset_mesh.prepare_for_use();
}

void CoilBoundaryGenerator::changeMeshBasis(libMesh::Mesh &input_mesh,
                                            const Eigen::Vector3d &new_origin,
                                            const Eigen::Matrix3d &new_basis,
                                            const Eigen::Vector3d &old_origin,
                                            const Eigen::Matrix3d &old_basis) {
  libMesh::Mesh mesh_copy(input_mesh);
  input_mesh.clear();

  for (auto &node : mesh_copy.node_ptr_range()) {
    // Eigen::Vector to store node coords
    Eigen::Vector3d point, new_point;

    for (u_int i = 0; i < 3; i++) {
      point(i) = (*node)(i);
    }

    new_point = calculateLocalCoords(point, new_origin, new_basis, old_origin,
                                     old_basis);
    // new_point = calculateLocalCoords(basisMatrix, origin, origin);
    // std::cout << new_point.transpose() << std::endl;

    double pnt[3];
    for (u_int i = 0; i < 3; i++) {
      pnt[i] = new_point(i);
    }

    libMesh::Point xyz(pnt[0], pnt[1], pnt[2]);
    // std::cout << pnt[0] << " " << pnt[1] << " " << pnt[2] << std::endl;
    input_mesh.add_point(xyz, node->id());
  }

  for (auto &elem : mesh_copy.element_ptr_range()) {
    libMesh::Elem *new_elem = libMesh::Elem::build(elem->type()).release();
    for (int j = 0; j < elem->n_nodes(); j++) {
      new_elem->set_node(j) = input_mesh.node_ptr(elem->node_ref(j).id());
    }
    input_mesh.add_elem(new_elem);
  }
  input_mesh.prepare_for_use();
}

// Change mesh basis for a libIGL (Eigen) mesh
void CoilBoundaryGenerator::changeMeshBasis(Eigen::MatrixXd &vertices,
                                            const Eigen::Vector3d &new_origin,
                                            const Eigen::Matrix3d &new_basis,
                                            const Eigen::Vector3d &old_origin,
                                            const Eigen::Matrix3d &old_basis) {
  // Inverse the new basis matrix
  Eigen::Matrix3d new_basis_inverse = new_basis.inverse();
  // We assume we are changing basis from bog standard cartesian coords.
  //  i.e. an origin of 0,0,0 and basis vectors of (1,0,0), (0,1,0), (0,0,1)
  // Eigen::Matrix3d old_basis(Eigen::Matrix3d::Identity());
  // Eigen::Vector3d old_origin(Eigen::Vector3d::Zero());
  // Do row wise operations on
  for (int i = 0; i < vertices.rows(); i++) {
    vertices.row(i) =
        new_basis_inverse *
        (old_origin - new_origin + (old_basis * vertices.row(i).transpose()));
  }
}

void CoilBoundaryGenerator::genRemainingFiveBoundaryFaces(
    Eigen::MatrixXd &tri_vertices, Eigen::MatrixXi &tri_elems, double length,
    int subdivisions, std::string tri_flags) {
  Eigen::MatrixXd border_verts(4 * subdivisions, 2);
  Eigen::MatrixXi border_elems(4 * subdivisions, 2);

  Eigen::MatrixXd square_verts(4 * subdivisions, 2);
  Eigen::MatrixXi square_elems(4 * subdivisions, 2);

  // tri_vertices(4 * subdivisions, 2);
  // tri_elems(4 * subdivisions, 3);

  Eigen::MatrixXd seeds;

  genSquare(border_verts, border_elems, length, subdivisions);

  igl::triangle::triangulate(border_verts, border_elems, seeds, tri_flags,
                             square_verts, square_elems);

  // Rotational matrices to rotate elements
  Eigen::Matrix3d x_rot_base;
  x_rot_base << 1, 0, 0, 0, 0, -1, 0, 1, 0;

  Eigen::Matrix3d y_rot_base;
  y_rot_base << 0, 0, -1, 0, 1, 0, 1, 0, 0;

  // Resize tri_vertices to work in 3d, as it is currently a 2D mesh,
  //  this is done just by adding a column of zeroes as our "z" coord
  square_verts.conservativeResize(square_verts.rows(), square_verts.cols() + 1);
  square_verts.col(square_verts.cols() - 1) =
      Eigen::VectorXd::Zero(square_verts.rows());

  // Define our rotation matrices
  std::vector<Eigen::Matrix3d> rot_matrices = {x_rot_base, y_rot_base};
  std::vector<Eigen::MatrixXd> new_faces(5, square_verts);

  square_verts.col(square_verts.cols() - 1) =
      Eigen::VectorXd::Constant(square_verts.rows(), length);
  // tri_elems.conservativeResize(tri_elems.rows(), tri_elems.cols()+1);
  // tri_elems.col(tri_elems.cols()-1) =
  // Eigen::VectorXi::Zero(tri_elems.rows());

  for (int i = 0; i < 4; i++) {
    new_faces[i] *= rot_matrices[(i % 2)];
  }

  translateMesh(new_faces[0], {0, length / 2, length / 2});
  translateMesh(new_faces[1], {length / 2, 0, length / 2});
  translateMesh(new_faces[2], {0, -length / 2, length / 2});
  translateMesh(new_faces[3], {-length / 2, 0, length / 2});
  translateMesh(new_faces[4], {0, 0, length});

  for (int i = 0; i < 5; i++) {
    combineMeshes(boundary_face_merge_tolerance_, tri_vertices, tri_elems,
                  new_faces[i], square_elems);
  }
}

void CoilBoundaryGenerator::getCoplanarSeedPoints(
    libMesh::Mesh &mesh, Eigen::MatrixXd &seed_points) {

  // Because of annoying libMesh methods, we need to make sets to store the
  // sideset ID's
  std::set<libMesh::boundary_id_type> sideset_one_id, sideset_two_id;
  sideset_one_id.insert(coil_sideset_one_id);
  sideset_two_id.insert(coil_sideset_two_id);

  // Generate separate meshes for two coil sidesets
  libMesh::Mesh sideset_one_mesh(mesh.comm());
  libMesh::Mesh sideset_two_mesh(mesh.comm());
  mesh.boundary_info->sync(sideset_one_id, sideset_one_mesh);
  mesh.boundary_info->sync(sideset_two_id, sideset_two_mesh);

  // As soon as an element not on the boundary is found, use one of its nodes
  //  as a seeding point
  for (auto &elem : sideset_one_mesh.active_element_ptr_range()) {
    if (!(elem->on_boundary())) {
      for (u_int i = 0; i < 3; i++) {
        seed_points.row(0)(i) = elem->node_ref(0)(i);
      }
      break;
    }
  }

  // As soon as an element not on the boundary is found, use one of its nodes
  //  as a seeding point
  for (auto &elem : sideset_two_mesh.active_element_ptr_range()) {
    if (!(elem->on_boundary())) {
      for (u_int i = 0; i < 3; i++) {
        seed_points.row(1)(i) = elem->node_ref(0)(i);
      }
      break;
    }
  }
}
