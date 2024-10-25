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


  /** Method generates a rectangular mesh comprised of tri elements.
   \c verts  will be populated with the vertex data, \c elems will be populated
   with the element connectivity data. \c x_dim and \c y_dim will allow the
   user to change the length of the edges of the square. \c x_subdiv and \c y_subdiv will allow
   you to set how many elements there should be per edge*/
  void genTriangulatedRect(Eigen::MatrixXd &rect_verts,
               Eigen::MatrixXi &rect_elems, double x_dim, double y_dim,
               int x_subdiv, int y_subdiv, std::string tri_flags);

  /** Method generates a rectangular boundary comprised of just edge elements, i.e. the perimeter of the rectangle.
 \c verts  will be populated with the vertex data, \c elems will be populated
  with the element connectivity data. \c x_dim and \c y_dim will allow the
   user to change the length of the edges of the square. \c x_subdiv and \c y_subdiv will allow
   you to set how many elements there should be per edge*/
  void genRect(Eigen::MatrixXd &edge_verts,
               Eigen::MatrixXi &edge_elems, double x_dim, double y_dim,
               int x_subdiv, int y_subdiv);

  /** Method generates a square boundary comprised of just edge elements.
   \c verts  will be populated with the vertex data, \c elems will be populated
   with the element connectivity data. \c length and \c subdivisions allow the
   user to change the length of the edges of the square, as well as how many
   elements there should be per edge*/
  void genSquare(Eigen::MatrixXd &edge_verts, Eigen::MatrixXi &edge_elems, double length,
                 int subdivisions);

    /** Method generates a square boundary comprised of just edge elements.
   \c verts  will be populated with the vertex data, \c elems will be populated
   with the element connectivity data. \c length and \c subdivisions allow the
   user to change the length of the edges of the square, as well as how many
   elements there should be per edge*/
  void genTriangulatedSquare(Eigen::MatrixXd &edge_verts, Eigen::MatrixXi &edge_elems, double length,
                 int subdivisions, std::string tri_flags);


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
  void genBoundary(Eigen::MatrixXd &boundary_vertices,
                   Eigen::MatrixXi &boundary_elements, double length,
                   int subdivisions, std::string tri_flags);

  /** Generates a cuboid boundary based off of the bounding box dimensions of the input mesh */
  void genBoundingBoxBoundary(Eigen::MatrixXd &boundary_vertices,
                              Eigen::MatrixXi &boundary_elements,
                              double scaling_x, double scaling_y, 
                              double scaling_z, int subdivisions,
                              std::string tri_flags);


  /** Function overload that accepts one scaling factor for all dimensions */
  void genBoundingBoxBoundary(Eigen::MatrixXd &boundary_vertices,
                              Eigen::MatrixXi &boundary_elements,
                              double scaling_factor, int subdivisions,
                              std::string tri_flags);

  void addBoundingBoxBoundary(double scaling_x, double scaling_y, double scaling_z,
                              int subdivisions, std::string tri_flags);

  void translateMesh(Eigen::MatrixXd &verts, Eigen::Vector3d translationVector);

  void translateMesh(Eigen::MatrixXd &verts, std::vector<double> &translationVector);
  
  void getBoundaryDimensions(double scaling_factor);

  void getBoundaryDimensions(double scaling_x, double scaling_y, double scaling_z);

  /**Checks to see if the boundary will overlap with component mesh.*/
  void checkBoundary(const double &length);

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

  std::vector<double> centroid_;

  double bb_x_dim, bb_y_dim, bb_z_dim;

private:
};
