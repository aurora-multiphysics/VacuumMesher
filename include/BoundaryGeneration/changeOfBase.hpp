#include "BoundaryGeneration/generateBoundaries.hpp"
#include "Tetrahedralisation/removeDupeNodes.hpp"
#include "igl/triangle/triangulate.h"

void getBasisChangeMesh(libMesh::Mesh& mesh, Eigen::MatrixXd& bound_verts, Eigen::MatrixXi& bound_elems, double length, int subdivisions, double max_el_size);

bool getBasisMatrix(Eigen::Matrix3d& basisMatrix, Eigen::Matrix3d& plane_points);

Eigen::Vector3d calculateLocalCoords(Eigen::Vector3d& point, Eigen::Vector3d newOrigin, Eigen::Matrix3d newBasis, Eigen::Vector3d oldOrigin = {0, 0, 0}, Eigen::Matrix3d oldBasis = Eigen::Matrix3d::Identity());

void doubleCheck(Eigen::Matrix3d& basisMatrix, Eigen::Vector3d& origin, Eigen::Vector3d& point, Eigen::Vector3d& initialPoint);

void generateCoilFaceBound(Eigen::MatrixXd& V, Eigen::MatrixXi& F, Eigen::MatrixXd& holes, Eigen::MatrixXd& tri_V, Eigen::MatrixXi& tri_F, double length, int subdivisions, double max_elem_size);

void generateCoilFaceBound(libMesh::Mesh& mesh, libMesh::Mesh& outputMesh, libMesh::Mesh& remainingBoundary, Eigen::MatrixXd& holes);

void genSidesetMesh(libMesh::Mesh& mesh, libMesh::Mesh& sidesetMesh, std::vector<std::string> ssNames = {"coil_in", "coil_out"});

void genSidesetBounds(Eigen::MatrixXd& V, Eigen::MatrixXi& F, double length, int subdivisions);

void genSidesetBounds(libMesh::Mesh& sidesetMesh, libMesh::Mesh& remainingBoundary);

void changeMeshBasis(libMesh::Mesh& mesh, Eigen::Vector3d newOrigin, Eigen::Matrix3d newBasis, Eigen::Vector3d oldOrigin = {0, 0, 0}, Eigen::Matrix3d oldBasis = Eigen::Matrix3d::Identity());

// Function overload for use with libIGL (Eigen) meshes
void changeMeshBasis(Eigen::MatrixXd& V, Eigen::Vector3d newOrigin, Eigen::Matrix3d newBasis, Eigen::Vector3d oldOrigin = {0, 0, 0}, Eigen::Matrix3d oldBasis = Eigen::Matrix3d::Identity());

void getCoplanarSeedPoints(libMesh::Mesh& mesh, Eigen::MatrixXd& seed_points, std::string ss1_name = "coil_in", std::string ss2_name = "coil_out");

void genSquare(Eigen::MatrixXd& V, Eigen::MatrixXi& F, double length, int subdivisions);

void combineIGLMeshes(Eigen::MatrixXd& V1, Eigen::MatrixXi& F1, Eigen::MatrixXd& V2, Eigen::MatrixXi& F2);

void genRemainingBoundary(Eigen::MatrixXd& V_tri, Eigen::MatrixXi& F_tri, double length, int subdivisions, double max_elem_size, double tol);
