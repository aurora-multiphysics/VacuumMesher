#include "BoundaryGeneration/boundaryGeneration.hpp"

class CoilBoundaryGenerator : public BoundaryGenerator {
public:
  /** Method to generate the boundary for a coil. A method exists specifically
for coils, as often the boundary needs to be coplanar with some coil "in"
and "out" sidesets. This method will generate the correct boundary for a
problem of this nature. The method takes in your starting \c mesh, two Eigen
objects \c boundVerts and \c boundElems that represent the output boundary
vertices and faces (vertice connectivity data)*/
  void generateCoilBoundary(libMesh::Mesh &mesh, libMesh::Mesh &boundaryMesh,
                            double length, int subdivisions,
                            std::string triSettings);

  /** Method takes in an Eigen::Matrix "basisMatrix" by reference. This matrix
will be populated with the basis vectors for a cartesian coordinate system.
The new systems "XY" plane is defined by 3 points contained within
"planePoints". I need to add a check in that makes sure these 3 points are
not on the same line, and hence properly define a plane. This makes this new
plane the Z=0 point of the new system. "basisMatrix" is population with the
basis vectors for the new system, organised by column (x basis = column 1
etc.)*/
  bool getBasisMatrix(Eigen::Matrix3d &basisMatrix,
                      Eigen::Matrix3d &planePoints);

  /** Method that calculates the coordinate positions in a set of new
   basis vectors \c newBasis. \c NewBasis contains the new basis vectors with
   each column representing the X Y and Z basis' respectively.
   The default arguments assume that you are moving your point from the
   "standard" cartesian space (origin = 0,0,0  x_basis = 1, 0, 0 etc.)*/
  Eigen::Vector3d
  calculateLocalCoords(Eigen::Vector3d &point, Eigen::Vector3d newOrigin,
                       Eigen::Matrix3d newBasis,
                       Eigen::Vector3d oldOrigin = {0, 0, 0},
                       Eigen::Matrix3d oldBasis = Eigen::Matrix3d::Identity());

  /** Method for generating the face of the cubic boundary that is coplanar with
the coil sidesets. Here \c verts and  \c elems are the vertices and
faces(connectivity data) for the input mesh, which is probably composed of
2D edge elements. \c holes is a matric which contains any seeding points
used for the triangulation of the closed space described by \c V and \c F.
\c triV and \c triF are the data structures where the data for the output
tri mesh will be stored.*/
  void generateCoilFaceBound(Eigen::MatrixXd &verts, Eigen::MatrixXi &elems,
                             Eigen::MatrixXd &holes, Eigen::MatrixXd &triVerts,
                             Eigen::MatrixXi &triElems, double length,
                             int subdivisions, std::string triSettings);

  /** Method for generating the face of the cubic boundary that is coplanar with
     the coil sidesets. Here V and F are the vertices and faces(connectivity
     data) for the input mesh, which is probably composed of 2D edge elements.
     holes is a matric which contains any seeding points used for the
     triangulation of the closed space described by V and F. triV and triF are
     the data structures where the data for the output tri mesh will be
     stored.*/
  void generateCoilFaceBound(libMesh::Mesh &mesh, libMesh::Mesh &outputMesh,
                             libMesh::Mesh &remainingBoundary,
                             Eigen::MatrixXd &holes);
  /***/
  void genSidesetMesh(libMesh::Mesh &mesh, libMesh::Mesh &sidesetMesh,
                      std::vector<std::string> ssNames = {"coil_in",
                                                          "coil_out"});
  /***/
  void genSidesetBounds(Eigen::MatrixXd &verts, Eigen::MatrixXi &elems,
                        double length, int subdivisions);

  void genSidesetBounds(libMesh::Mesh &sidesetMesh,
                        libMesh::Mesh &remainingBoundary);

  /** Rotate/translate a \c mesh according to a set of new basis vectors \c
   * newBasis, and a new origin \c newOrigin. */
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

  /** For our coil problem, to generate the part of the boundary that is
   coplanar with the coil sidesets correctly*/
  void getCoplanarSeedPoints(libMesh::Mesh &mesh, Eigen::MatrixXd &seedPoints,
                             std::string ss1Name = "coil_in",
                             std::string ss2Name = "coil_out");

protected:
private:
};