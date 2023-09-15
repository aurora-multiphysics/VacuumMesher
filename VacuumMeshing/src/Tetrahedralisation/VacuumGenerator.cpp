#include "Tetrahedralisation/VacuumGenerator.hpp"

VacuumGenerator(libMesh::Mesh &boundary_mesh, libMesh::Mesh &vacuum_mesh)
    : boundary_mesh_(boundary_mesh), vacuum_mesh_(vacuum_mesh) {}

VacuumGenerator::~VacuumGenerator(){};

// Takes in seedPoints arguments for meshes with internal cavities
// , this will be most vacuum meshes I think?
void VacuumGenerator::tetrahedraliseVacuumRegion(Eigen::MatrixXd &seedPoints,
                                                 std::string tet_settings) {
  Eigen::MatrixXd V;
  Eigen::MatrixXi F;
  libMeshToIGL(boundary_mesh_, V, F);

  Eigen::MatrixXd R;
  Eigen::MatrixXd TV;
  Eigen::MatrixXi TT;
  Eigen::MatrixXi TF;
  Eigen::MatrixXd TR;
  Eigen::MatrixXi TN;
  Eigen::MatrixXi PT;
  Eigen::MatrixXi FT;
  size_t numRegions;
  igl::copyleft::tetgen::tetrahedralize(V, F, seedPoints, R, tet_settings, TV,
                                        TT, TF, TR, TN, PT, FT, numRegions);

  IGLToLibMesh(vacuum_mesh_, TV, TT);
}

void VacuumGenerator::tetrahedraliseVacuumRegion(Eigen::MatrixXd &V,
                                                 Eigen::MatrixXi &F,
                                                 Eigen::MatrixXd &seedPoints,
                                                 std::string tet_settings) {
  Eigen::MatrixXd R;
  Eigen::MatrixXd TV;
  Eigen::MatrixXi TT;
  Eigen::MatrixXi TF;
  Eigen::MatrixXd TR;
  Eigen::MatrixXi TN;
  Eigen::MatrixXi PT;
  Eigen::MatrixXi FT;
  size_t numRegions;
  igl::copyleft::tetgen::tetrahedralize(V, F, seedPoints, R, tet_settings, TV,
                                        TT, TF, TR, TN, PT, FT, numRegions);

  IGLToLibMesh(vacuum_mesh_, TV, TT);
}

// Version of the function primarily built for use with coils. When generating
// the vacuum mesh for coil geometry , there is no "internal cavity". The
// internal geometry for the coil is already represented in the boundary mesh.
void VacuumGenerator::tetrahedraliseVacuumRegion(libMesh::Mesh &boundaryMesh,
                                                 libMesh::Mesh &vacuumMesh,
                                                 std::string tet_settings) {
  Eigen::MatrixXd V;
  Eigen::MatrixXi F;
  libMeshToIGL(boundaryMesh, V, F);

  Eigen::MatrixXd seedPoints;
  Eigen::MatrixXd R;
  Eigen::MatrixXd TV;
  Eigen::MatrixXi TT;
  Eigen::MatrixXi TF;
  Eigen::MatrixXd TR;
  Eigen::MatrixXi TN;
  Eigen::MatrixXi PT;
  Eigen::MatrixXi FT;
  size_t numRegions;

  igl::copyleft::tetgen::tetrahedralize(V, F, seedPoints, R, tet_settings, TV,
                                        TT, TF, TR, TN, PT, FT, numRegions);

  IGLToLibMesh(vacuumMesh, TV, TT);
}

Eigen::MatrixXd VacuumGenerator::getSeeds(libMesh::Mesh mesh, double tol) {
  Eigen::MatrixXd V;
  Eigen::MatrixXi F;
  // Oreintated faces
  libMeshToIGL(mesh, V, F);

  Eigen::MatrixXd N_faces;

  Eigen::MatrixXd seed_points(F.rows(), 3);

  igl::per_face_normals(V, F, N_faces);

  for (int i; i < N_faces.rows(); i++) {
    std::vector<double> seed(3, 0);
    std::vector<double> normal = {N_faces.row(i)[0], N_faces.row(i)[1],
                                  N_faces.row(i)[2]};
    std::vector<int> verts = {F.row(i)[0], F.row(i)[1], F.row(i)[2]};
    for (auto &vert : verts) {
      seed[0] += (V.row(vert)[0] - tol * normal[0]) / 3; // x component of seed
      seed[1] += (V.row(vert)[1] - tol * normal[1]) / 3; // y component of seed
      seed[2] += (V.row(vert)[2] - tol * normal[2]) / 3; // z component of seed
    }
    seed_points.row(i) << seed[0], seed[1], seed[2];
  }

  return seed_points;
}