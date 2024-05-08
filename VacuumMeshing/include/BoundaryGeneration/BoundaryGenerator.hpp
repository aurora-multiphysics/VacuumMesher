/** \file
 * Generating the correct boundary around the original mesh part is necessary
 * for generating the desired vacuum mesh. This file contains methods pertaining
 * to doing exactly that.
 */
#pragma once
#include "Utils/utils.hpp"
#include "igl/triangle/triangulate.h"
#include "libmesh/boundary_info.h"
#include "libmesh/elem.h"
#include "libmesh/enum_elem_type.h"
#include "libmesh/enum_io_package.h"
#include "libmesh/libmesh.h"
#include "libmesh/mesh.h"
#include "libmesh/mesh_tools.h"

class BoundaryGenerator {
public:
  /**
   * Default destructor
   */
  ~BoundaryGenerator();

  /**
   * Constructor
   */
  BoundaryGenerator(libMesh::Mesh &mesh, libMesh::Mesh &surface_mesh,
                    libMesh::Mesh &boundary_mesh,
                    double mesh_merge_tolerance = 0);

  /** Adds a boundary to the \c skinnedMesh. This new mesh with the boundary
 added is stored in the \c boundaryMesh. The \c length and number of mesh \c
 subdivisions are required inputs, as well as the \c triSettings that are input
 to the triangle lib calls */
  virtual void addBoundary(double length, int subdivisions,
                           std::string tri_flags);

  /** Method generates a square boundary comprised of just edge elements.
   \c verts  will be populated with the vertex data, \c elems will be populated
   with the element connectivity data. \c length and \c subdivisions allow the
   user to change the length of the edges of the square, as well as how many
   elements there should be per edge*/
  void genSquare(Eigen::MatrixXd &verts, Eigen::MatrixXi &elems, double length,
                 int subdivisions);

  /** Method for combining two libigl meshes that you are SURE do NOT intersect
   and DO NOT have duplicate nodes. There is a combine meshes method in
   removeDupeNodes.cpp that can combine meshes with duplicate nodes using an
   rTree data structure, so if you need that, use that. \p vertsOne and \p
   elemsOne are the vertice and element data structures for the first mesh, and
   \p vertsOne and \p elemsTwo represent the second mesh. The meshes will be
   merged into the first mesh (V1, F1)*/
  void combineIGLMeshes(Eigen::MatrixXd &vertices_one,
                        Eigen::MatrixXi &elements_one,
                        Eigen::MatrixXd &vertices_two,
                        Eigen::MatrixXi &elements_two);

  // Generates a cubic boundary used to define a vacuum region for
  // tetrahedralisation
  void genBoundary(Eigen::MatrixXd &triangle_vertices,
                   Eigen::MatrixXi &triangle_elements, double length,
                   int subdivisions, std::string tri_flags);

  void translateMesh(Eigen::MatrixXd &verts, Eigen::Vector3d translationVector);

  /**Checks to see if the boundary will overlap with component mesh.*/
  void checkBoundary(const double &length) const;

protected:
  // Method used to get tolerances automatically
  void setMergeToleranceAuto();

  // Libmesh meshes to store the meshes we need
  libMesh::Mesh &mesh_, &surface_mesh_, &boundary_mesh_;

  // Eigen data structures to store mesh data for our boundaryz
  // Eigen::MatrixXd boundary_verts;
  // Eigen::MatrixXi boundary_elems;
  double mesh_merge_tolerance_;
  double boundary_face_merge_tolerance_ = 1e-06;

private:
};
