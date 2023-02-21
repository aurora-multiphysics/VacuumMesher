#pragma once
#include "BoundaryGeneration/boundaryGeneration.hpp"
#include "Tetrahedralisation/removeDupeNodes.hpp"
#include "igl/triangle/triangulate.h"

/** Method to generate the boundary for a coil. A method exists specifically for
  coils, as often the boundary needs to be coplanar with some coil "in" and
  "out" sidesets. This method will generate the correct boundary for a problem
  of this nature. The method takes in your starting mesh "mesh", two Eigen
  objects "boundVerts" and "boundElems" that represent the output boundary
  vertices and faces (vertice connectivity data)*/
void generateCoilBoundary(libMesh::Mesh &mesh, Eigen::MatrixXd &boundVerts,
                          Eigen::MatrixXi &boundElems, double length,
                          int subdivisions, std::string triSettings);

/** Method takes in an Eigen::Matrix "basisMatrix" by reference. This matrix
 will be populated with the basis vectors for a cartesian coordinate system. The
 new systems "XY" plane is defined by 3 points contained within "planePoints". I
 need to add a check in that makes sure these 3 points are not on the same line,
 and hence properly define a plane. This makes this new plane the Z=0 point of
 the new system. "basisMatrix" is population with the basis vectors for the new
 system, organised by column (x basis = column 1 etc.)*/
bool getBasisMatrix(Eigen::Matrix3d &basisMatrix, Eigen::Matrix3d &planePoints);

/** Method that calculates the coordinates of a set of coordinates in a new
 basis. The default arguments assume that you are moving your point from the
 "standard" cartesian space (origin = 0,0,0  x_basis = 1, 0, 0 etc.)*/
Eigen::Vector3d
calculateLocalCoords(Eigen::Vector3d &point, Eigen::Vector3d newOrigin,
                     Eigen::Matrix3d newBasis,
                     Eigen::Vector3d oldOrigin = {0, 0, 0},
                     Eigen::Matrix3d oldBasis = Eigen::Matrix3d::Identity());

//
void doubleCheck(Eigen::Matrix3d &basisMatrix, Eigen::Vector3d &origin,
                 Eigen::Vector3d &point, Eigen::Vector3d &initialPoint);

//
void generateCoilFaceBound(Eigen::MatrixXd &V, Eigen::MatrixXi &F,
                           Eigen::MatrixXd &holes, Eigen::MatrixXd &triV,
                           Eigen::MatrixXi &triF, double length,
                           int subdivisions, std::string tri_settings,
                           libMesh::Mesh &test);

//
void generateCoilFaceBound(libMesh::Mesh &mesh, libMesh::Mesh &outputMesh,
                           libMesh::Mesh &remainingBoundary,
                           Eigen::MatrixXd &holes);
//
void genSidesetMesh(libMesh::Mesh &mesh, libMesh::Mesh &sidesetMesh,
                    std::vector<std::string> ssNames = {"coil_in", "coil_out"});
//
void genSidesetBounds(Eigen::MatrixXd &V, Eigen::MatrixXi &F, double length,
                      int subdivisions);

void genSidesetBounds(libMesh::Mesh &sidesetMesh,
                      libMesh::Mesh &remainingBoundary);

/** Rotate/translate a mesh according to a set of new basis vectors, and a new
 * origin.*/
void changeMeshBasis(libMesh::Mesh &mesh, Eigen::Vector3d newOrigin,
                     Eigen::Matrix3d newBasis,
                     Eigen::Vector3d oldOrigin = {0, 0, 0},
                     Eigen::Matrix3d oldBasis = Eigen::Matrix3d::Identity());

/** Rotate/translate a mesh according to a set of new basis vectors, and a new
   origin. Function overload for use with libIGL (Eigen) meshes*/
void changeMeshBasis(Eigen::MatrixXd &V, Eigen::Vector3d newOrigin,
                     Eigen::Matrix3d newBasis,
                     Eigen::Vector3d oldOrigin = {0, 0, 0},
                     Eigen::Matrix3d oldBasis = Eigen::Matrix3d::Identity());

/** For our coil problem, to generate the part of the boundary that is coplanar
 with the coil sidesets correctly*/
void getCoplanarSeedPoints(libMesh::Mesh &mesh, Eigen::MatrixXd &seedPoints,
                           std::string ss1Name = "coil_in",
                           std::string ss2Name = "coil_out");

/** Method generates a square boundary comprised of just edge elements. The
 verts argument will be populated with the vertex data, elems will be populated
 with the element connectivity data and length and subdivisions allow the user
 to change the length of the edges of the square, as well as how many elements
 there should be per "edge"*/
void genSquare(Eigen::MatrixXd &verts, Eigen::MatrixXi &elems, double length,
               int subdivisions);

/** Method for combining two libigl meshes that you are SURE do NOT intersect/
 have duplicate nodes. There is a combine meshes method in removeDupeNodes.cpp
 that can combine meshes with duplicate nodes using an rTree data structure, so
 if you need that, use that. Here V1 and F1 are the vertice and element data
 structures for the first mesh, and V2 and F2 represent the second mesh. The
 meshes will be merged into the first mesh (V1, F1)*/
void combineIGLMeshes(Eigen::MatrixXd &V1, Eigen::MatrixXi &F1,
                      Eigen::MatrixXd &V2, Eigen::MatrixXi &F2);

/** Generates the 5 remaining faces of a cubic boundary*/
void genRemainingBoundary(Eigen::MatrixXd &vTri, Eigen::MatrixXi &fTri,
                          double length, int subdivisions,
                          std::string triSettings, double tol);
