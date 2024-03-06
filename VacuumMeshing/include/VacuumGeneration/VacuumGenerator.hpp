#pragma once

#include "Utils/libmeshConversions.hpp"
#include "libmesh/mesh.h"
#include <filesystem>
#include <igl/copyleft/tetgen/tetrahedralize.h>
#include <igl/per_face_normals.h>
#include <igl/slice.h>
#include <iostream>
#include <unistd.h>

class VacuumGenerator {
public:
  VacuumGenerator(libMesh::Mesh &mesh, libMesh::Mesh &surface_mesh,
                  libMesh::Mesh &boundary_mesh, libMesh::Mesh &vacuum_mesh,
                  std::multimap<unsigned int, unsigned int> *surface_face_map = nullptr);

  ~VacuumGenerator();

  void generateVacuumMesh(const std::string tet_settings);
  /** Method used to generate the tetrehedra between the boundary mesh and the
    part mesh i.e. generates the vacuum region mesh. This method uses the surface and
    boundary mesh passed to the constructor.
    
    The seed_points represent a set of coordinates that exist within closed
    surfaces you do NOT want tetrahedra to be generated in, and tetSettings is a
    string that gets passed to tetgen, the underlying library used for tet
    generation.*/
  void tetrahedraliseVacuumRegion(const std::string tet_settings);

  /** Method used to generate the tetrehedra between the boundary mesh and the
    part mesh i.e. generates the vacuum mesh. This overload takes in the boundary and 
    surface mesh in the eigen objects vertices and element_connectivity. It does not make use of
    the meshes passed into the constructor. The method only still exists as it's
    useful for debugging sometimes. 

    The seed_points represent a set of coordinates that exist within closed
    surfaces you do NOT want tetrahedra to be generated in, and tetSettings is a
    string that gets passed to tetgen, the underlying library used for tet
    generation.*/
  void tetrahedraliseVacuumRegion(const Eigen::MatrixXd &vertices,
                                  const Eigen::MatrixXi &element_connectivity,
                                  const Eigen::MatrixXd &seed_points,
                                  const std::string tet_settings);

  /** Method used to generate the tetrehedra between the boundary mesh and the
    part mesh i.e. generates the vacuum mesh. This method takes in no seed points
    so isn't really that useful, but again it is helpful for debugging and testing so 
    it gets to stay.
   
    tetSettings is a string that gets passed to tetgen, the underlying library used for tet
    generation.*/
  void tetrahedraliseVacuumRegionNoCavity(const std::string tet_settings);

  // Get seeds
  Eigen::MatrixXd getSeeds(libMesh::Mesh mesh) const;

protected:
  // References to mesh objects passed in via constructor
  libMesh::Mesh &mesh_, &boundary_mesh_, &surface_mesh_, &vacuum_mesh_;

  // Tolerance value used for rTree merging
  double merge_tolerance_ = 1e-07;
  double seeding_tolerance_ = 1e-07;

  std::multimap<unsigned int, unsigned int>* surface_face_map_;

private:
};
