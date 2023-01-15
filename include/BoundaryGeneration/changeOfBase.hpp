#include "BoundaryGeneration/generateBoundaries.hpp"
#include "Tetrahedralisation/removeDupeNodes.hpp"
#include "igl/triangle/triangulate.h"

void getBasisChangeMesh(libMesh::Mesh& mesh, libMesh::Mesh& sidesetMesh,libMesh::Mesh& newMesh);

bool getBasisMatrix(Eigen::Matrix3d& basisMatrix, Eigen::Matrix3d& plane_points);

Eigen::Vector3d calculateLocalCoords(Eigen::Vector3d& point, Eigen::Vector3d newOrigin, Eigen::Matrix3d newBasis, Eigen::Vector3d oldOrigin = {0, 0, 0}, Eigen::Matrix3d oldBasis = Eigen::Matrix3d::Identity());

void doubleCheck(Eigen::Matrix3d& basisMatrix, Eigen::Vector3d& origin, Eigen::Vector3d& point, Eigen::Vector3d& initialPoint);

void generateCoilFaceBound(libMesh::Mesh& mesh, libMesh::Mesh& outputMesh, libMesh::Mesh& remainingBoundary, Eigen::MatrixXd& holes);

void genSidesetBounds(libMesh::Mesh& sidesetMesh, double length);

void genSidesetBounds(libMesh::Mesh& sidesetMesh, libMesh::Mesh& remainingBoundary);

void changeMeshBasis(libMesh::Mesh& mesh, Eigen::Vector3d newOrigin, Eigen::Matrix3d newBasis, Eigen::Vector3d oldOrigin = {0, 0, 0}, Eigen::Matrix3d oldBasis = Eigen::Matrix3d::Identity());

void getCoplanarSeedPoints(libMesh::Mesh& mesh, Eigen::MatrixXd& seed_points, std::string ss1_name = "coil_in", std::string ss2_name = "coil_out");