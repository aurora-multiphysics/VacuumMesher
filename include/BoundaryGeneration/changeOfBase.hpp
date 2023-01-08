#include "BoundaryGeneration/generateBoundaries.hpp"

void getBasisChangeMesh(libMesh::Mesh& mesh, libMesh::Mesh& sidesetMesh,libMesh::Mesh& newMesh);

bool getBasisMatrix(Eigen::Matrix3d& basisMatrix, Eigen::Matrix3d& plane_points);

Eigen::Vector3d calculateLocalCoords(Eigen::Matrix3d& rotationMatrix, Eigen::Vector3d& origin, Eigen::Vector3d& point);

void doubleCheck(Eigen::Matrix3d& basisMatrix, Eigen::Vector3d& origin, Eigen::Vector3d& point, Eigen::Vector3d& initialPoint);