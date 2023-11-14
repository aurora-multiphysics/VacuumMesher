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
  VacuumGenerator(
      libMesh::Mesh &mesh, libMesh::Mesh &surface_mesh,
      libMesh::Mesh &boundary_mesh, libMesh::Mesh &vacuum_mesh,
      const std::multimap<unsigned int, unsigned int> &surface_face_map);

  ~VacuumGenerator();

  void generateVacuumMesh(const std::string tet_settings);
  /** Method used to generate the tetrehedra between the boundary mesh and the
    part mesh i.e. generates the vacuum mesh. Here the boundaryMesh is the mesh
    you put into the function, a mesh composed of 2D facets you wish to generate
    tetrahedra in. The vacuumMesh is where the final tet mesh will be stores.
    The seed_points represent a set of coordinates that exist within closed
    surfaces you do NOT want tetrahedra to be generated in, and tetSettings is a
    string that gets passed to tetgen, the underlying library used for tet
    generation.*/
  void tetrahedraliseVacuumRegion(const Eigen::MatrixXd &seed_points,
                                  const std::string tet_settings);

  /** Method used to generate the tetrehedra between the boundary mesh and the
    part mesh i.e. generates the vacuum mesh. Here the boundaryMesh is the mesh
    you put into the function, a mesh composed of 2D facets you wish to generate
    tetrahedra in. The vacuumMesh is where the final tet mesh will be stores.
    The seed_points represent a set of coordinates that exist within closed
    surfaces you do NOT want tetrahedra to be generated in, and tetSettings is a
    string that gets passed to tetgen, the underlying library used for tet
    generation.*/
  void tetrahedraliseVacuumRegion(const Eigen::MatrixXd &V,
                                  const Eigen::MatrixXi &F,
                                  const Eigen::MatrixXd &seed_points,
                                  const std::string tet_settings);

  /** Method used to generate the tetrehedra between the boundary mesh and the
    part mesh i.e. generates the vacuum mesh. Here the boundaryMesh is the mesh
    you put into the function, a mesh composed of 2D facets you wish to generate
    tetrahedra in. The vacuumMesh is where the final tet mesh will be stores.
    The seed_points represent a set of coordinates that exist within closed
    surfaces you do NOT want tetrahedra to be generated in, and tetSettings is a
    string that gets passed to tetgen, the underlying library used for tet
    generation.*/
  void tetrahedraliseVacuumRegion(const std::string tet_settings);

  // Get seeds
  Eigen::MatrixXd getSeeds(libMesh::Mesh mesh) const;

protected:
  // References to mesh objects passed in via constructor
  libMesh::Mesh &mesh_, &boundary_mesh_, &surface_mesh_, &vacuum_mesh_;

  // Tolerance value used for rTree merging
  double merge_tolerance_ = 1e-07;
  double seeding_tolerance_ = 1e-07;

  std::shared_ptr<std::multimap<unsigned int, unsigned int>> surface_face_map_;

private:
};
