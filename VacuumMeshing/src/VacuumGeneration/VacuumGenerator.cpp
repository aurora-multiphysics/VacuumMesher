#include "VacuumGeneration/VacuumGenerator.hpp"
#include "Utils/removeDupeNodes.hpp"

VacuumGenerator::VacuumGenerator(
    libMesh::Mesh &mesh, libMesh::Mesh &surface_mesh,
    libMesh::Mesh &boundary_mesh, libMesh::Mesh &vacuum_mesh,
    std::multimap<unsigned int, unsigned int> *surface_face_map)
    : mesh_(mesh), surface_mesh_(surface_mesh), boundary_mesh_(boundary_mesh),
      vacuum_mesh_(vacuum_mesh), surface_face_map_(surface_face_map){

  // surface_face_map_ =
      // std::make_shared<std::multimap<unsigned int, unsigned int>>(
          // surface_face_map);
}

VacuumGenerator::~VacuumGenerator(){};

void VacuumGenerator::generateVacuumMesh(const std::string tet_settings) {
  // Create the mesh of the vacuumRegion
  tetrahedraliseVacuumRegion(tet_settings);

  // Temporary mesh, so that we don't have to edit the original
  libMesh::Mesh temp(mesh_);
  // combine

  combineMeshes(merge_tolerance_, temp, vacuum_mesh_, *surface_face_map_);
  vacuum_mesh_.clear();
  vacuum_mesh_ = libMesh::Mesh(temp);
}

// Takes in seed_points arguments for meshes with internal cavities, this will be most vacuum meshes 
void VacuumGenerator::tetrahedraliseVacuumRegion(const std::string tet_settings) {
  
  // Get coordinates of seed points. Seed points are points inside a cavity,
  //  They are needed so that the tetrahedralisation methods know where to not
  //  generate tets
  Eigen::MatrixXd seed_points(getSeeds(surface_mesh_));

  // Eigen objects to store mesh in libigl compatible format
  Eigen::MatrixXd vertices;
  Eigen::MatrixXi element_connectivity;

  // Populate eigen objects with mesh data
  libMeshToIGL(boundary_mesh_, vertices, element_connectivity);

  // Eigen data structures needed for tetrahedralize method to be run
  Eigen::MatrixXd R;
  Eigen::MatrixXd tet_vertices;
  Eigen::MatrixXi tet_connectivity;
  Eigen::MatrixXi TF;
  Eigen::MatrixXd TR;
  Eigen::MatrixXi TN;
  Eigen::MatrixXi PT;
  Eigen::MatrixXi FT;
  size_t num_regions;
  igl::copyleft::tetgen::tetrahedralize(vertices, element_connectivity, seed_points, R, tet_settings, tet_vertices,
                                        tet_connectivity, TF, TR, TN, PT, FT, num_regions);

  IGLToLibMesh(vacuum_mesh_, tet_vertices, tet_connectivity);
}

void VacuumGenerator::tetrahedraliseVacuumRegion(
    const Eigen::MatrixXd &vertices, const Eigen::MatrixXi &element_connectivity,
    const Eigen::MatrixXd &seed_points, const std::string tet_settings) {

  // Eigen data structures needed for tetrahedralize method to be run
  Eigen::MatrixXd R;
  Eigen::MatrixXd TV;
  Eigen::MatrixXi TT;
  Eigen::MatrixXi TF;
  Eigen::MatrixXd TR;
  Eigen::MatrixXi TN;
  Eigen::MatrixXi PT;
  Eigen::MatrixXi FT;
  size_t numRegions;
  igl::copyleft::tetgen::tetrahedralize(vertices, element_connectivity, seed_points, R, tet_settings, TV,
                                        TT, TF, TR, TN, PT, FT, numRegions);

  IGLToLibMesh(vacuum_mesh_, TV, TT);
}

// Version of the function primarily built for use with coils. When generating
// the vacuum mesh for coil geometry , there is no "internal cavity". The
// internal geometry for the coil is already represented in the boundary mesh.
void VacuumGenerator::tetrahedraliseVacuumRegionNoCavity(
    const std::string tet_settings) {

  Eigen::MatrixXd V;
  Eigen::MatrixXi F;
  libMeshToIGL(boundary_mesh_, V, F);

  // Eigen data structures needed for tetrahedralize method to be run
  Eigen::MatrixXd seed_points;
  Eigen::MatrixXd R;
  Eigen::MatrixXd TV;
  Eigen::MatrixXi TT;
  Eigen::MatrixXi TF;
  Eigen::MatrixXd TR;
  Eigen::MatrixXi TN;
  Eigen::MatrixXi PT;
  Eigen::MatrixXi FT;
  size_t numRegions;

  igl::copyleft::tetgen::tetrahedralize(V, F, seed_points, R, tet_settings, TV,
                                        TT, TF, TR, TN, PT, FT, numRegions);

  IGLToLibMesh(vacuum_mesh_, TV, TT);
}

Eigen::MatrixXd VacuumGenerator::getSeeds(const libMesh::Mesh mesh) const {
  Eigen::MatrixXd vertices;
  Eigen::MatrixXi element_connectivity;
  // Oreintated faces
  libMeshToIGL(mesh, vertices, element_connectivity);

  Eigen::MatrixXd N_faces;

  Eigen::MatrixXd seed_points(element_connectivity.rows(), 3);

  igl::per_face_normals(vertices, element_connectivity, N_faces);

  for (int i; i < N_faces.rows(); i++) {
    std::vector<double> seed(3, 0);
    std::vector<double> normal = {N_faces.row(i)[0], N_faces.row(i)[1],
                                  N_faces.row(i)[2]};
    std::vector<int> verts = {element_connectivity.row(i)[0], element_connectivity.row(i)[1], element_connectivity.row(i)[2]};
    for (auto &vert : verts) {
      seed[0] += (vertices.row(vert)[0] - seeding_tolerance_ * normal[0]) /
                 3; // x component of seed
      seed[1] += (vertices.row(vert)[1] - seeding_tolerance_ * normal[1]) /
                 3; // y component of seed
      seed[2] += (vertices.row(vert)[2] - seeding_tolerance_ * normal[2]) /
                 3; // z component of seed
    }
    seed_points.row(i) << seed[0], seed[1], seed[2];
  }

  return seed_points;
}