#pragma once

#include "SurfaceMeshing/SurfaceGenerator.hpp"
// Box struct that is inserted into the rTree. Each node gets it's own Box
// struct which is then
//  inserted.
struct Box {
  // Default constructor
  Box();
  // Constructor for combining libMesh nodes
  Box(std::array<double, 3> node, const double tol, unsigned int node_id) {
    centre = node;

    min = {node[0] - tol, node[1] - tol, node[2] - tol};

    max = {node[0] + tol, node[1] + tol, node[2] + tol};

    this->node_id = node_id;
  }

  // Constructor for inserting libIGL mesh
  Box(Eigen::Vector3d node, const double tol, unsigned int node_id) {
    centre = {node[0], node[1], node[2]};

    min = {node[0] - tol, node[1] - tol, node[2] - tol};

    max = {node[0] + tol, node[1] + tol, node[2] + tol};

    this->node_id = node_id;
  }

public:
  std::array<double, 3> centre; /// the midpoint of the box
  std::array<double, 3> min;    /// lowest coordinates of the box
  std::array<double, 3> max;    /// largest coorinates of the box
  unsigned int node_id;
};
