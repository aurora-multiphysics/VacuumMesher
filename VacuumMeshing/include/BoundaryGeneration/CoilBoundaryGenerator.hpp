#include "BoundaryGeneration/BoundaryGenerator.hpp"

class CoilBoundaryGenerator : public BoundaryGenerator {
public:
  // CoilBoundaryGenerator();

  CoilBoundaryGenerator(libMesh::Mesh &mesh, libMesh::Mesh &surface_mesh,
                        libMesh::Mesh &boundary_mesh,
                        const int sideset_one_id = 1,
                        const int sideset_two_id = 2,
                        double merge_tolerance = 0);

  ~CoilBoundaryGenerator();

  virtual void addBoundary(const double length, const int subdivisions,
                           const std::string tri_flags) override;

  /** Method to generate the boundary for a coil. A method exists specifically
for coils, as often the boundary needs to be coplanar with some coil "in"
and "out" sidesets. This method will generate the correct boundary for a
problem of this nature. The method takes in your starting \c mesh, two Eigen
objects \c boundVerts and \c boundElems that represent the output boundary
vertices and faces (vertice connectivity data)*/
  void generateCoilBoundary(const double length, const int subdivisions,
                            const std::string tri_flags);

  /** Method takes in an Eigen::Matrix "basis_matrix" by reference. This matrix
will be populated with the basis vectors for a cartesian coordinate system.
The new systems "XY" plane is defined by 3 points contained within
"plane_points". I need to add a check in that makes sure these 3 points are
not on the same line, and hence properly define a plane. This makes this new
plane the Z=0 point of the new system. "basis_matrix" is population with the
basis vectors for the new system, organised by column (x basis = column 1
etc.)*/
  bool getBasisMatrix(Eigen::Matrix3d &basis_matrix,
                      const Eigen::Matrix3d &plane_points);

  /** Method that calculates the coordinate positions in a set of new
   basis vectors \c new_basis. \c new_basis contains the new basis vectors with
   each column representing the X Y and Z basis' respectively.
   The default arguments assume that you are moving your point from the
   "standard" cartesian space (origin = 0,0,0  x_basis = 1, 0, 0 etc.)*/
  Eigen::Vector3d
  calculateLocalCoords(Eigen::Vector3d &point, Eigen::Vector3d new_origin,
                       Eigen::Matrix3d new_basis,
                       Eigen::Vector3d old_origin = {0, 0, 0},
                       Eigen::Matrix3d old_basis = Eigen::Matrix3d::Identity());

  /** Method for generating the face of the cubic boundary that is coplanar with
the coil sidesets. Here \c verts and  \c elems are the vertices and
faces(connectivity data) for the input mesh, which is probably composed of
2D edge elements. \c holes is a matric which contains any seeding points
used for the triangulation of the closed space described by \c V and \c F.
\c triV and \c triF are the data structures where the data for the output
tri mesh will be stored.*/
  void generateCoilFaceBound(const Eigen::MatrixXd &verts,
                             const Eigen::MatrixXi &elems,
                             const Eigen::MatrixXd &holes,
                             Eigen::MatrixXd &tri_vertices,
                             Eigen::MatrixXi &tri_elems, double length,
                             int subdivisions, std::string tri_flags);

  /** Method for generating the face of the cubic boundary that is coplanar with
     the coil sidesets. Here V and F are the vertices and faces(connectivity
     data) for the input mesh, which is probably composed of 2D edge elements.
     holes is a matric which contains any seeding points used for the
     triangulation of the closed space described by V and F. triV and triF are
     the data structures where the data for the output tri mesh will be
     stored.*/
  void generateCoilFaceBound(libMesh::Mesh &mesh, libMesh::Mesh &output_mesh,
                             libMesh::Mesh &boundary_without_coilside_face,
                             Eigen::MatrixXd &holes, std::string &tri_args);
  /***/
  void genSidesetMesh(libMesh::Mesh &mesh, libMesh::Mesh &sideset_mesh);
  /***/
  void genSidesetBounds(Eigen::MatrixXd &verts, Eigen::MatrixXi &elems,
                        const double length, const int subdivisions);

  void genSidesetBounds(libMesh::Mesh &sideset_mesh,
                        libMesh::Mesh &remaining_boundary);

  /** Rotate/translate a \c mesh according to a set of new basis vectors \c
   * new_basis, and a new origin \c new_origin. */
  void changeMeshBasis(
      libMesh::Mesh &mesh, const Eigen::Vector3d &new_origin,
      const Eigen::Matrix3d &new_basis,
      const Eigen::Vector3d &old_origin = {0, 0, 0},
      const Eigen::Matrix3d &old_basis = Eigen::Matrix3d::Identity());

  /** Rotate/translate a mesh according to a set of new basis vectors, and a new
     origin. Function overload for use with libIGL (Eigen) meshes*/
  void changeMeshBasis(
      Eigen::MatrixXd &vertices, const Eigen::Vector3d &new_origin,
      const Eigen::Matrix3d &new_basis,
      const Eigen::Vector3d &old_origin = {0, 0, 0},
      const Eigen::Matrix3d &old_basis = Eigen::Matrix3d::Identity());

  /** Generates the 5 remaining faces of a cubic boundary*/
  void genRemainingFiveBoundaryFaces(Eigen::MatrixXd &triVerts,
                                     Eigen::MatrixXi &triElems, double length,
                                     int subdivisions, std::string tri_flags);

  /** For our coil problem, to generate the part of the boundary that is
   coplanar with the coil sidesets correctly*/
  void getCoplanarSeedPoints(libMesh::Mesh &mesh, Eigen::MatrixXd &seed_points);

protected:
private:
  const int coil_sideset_one_id;
  const int coil_sideset_two_id;
};