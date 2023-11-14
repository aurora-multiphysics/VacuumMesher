#include "Utils/divConq.hpp"

namespace VacuumMesher {

ClosestPairFinder::ClosestPairFinder(libMesh::Mesh &mesh_) {
  mesh = std::make_shared<libMesh::Mesh>(mesh_);
  initialiseArrays();
}

double ClosestPairFinder::closestPair2D(std::vector<libMesh::Node *> &X) {
  // Create a copy of X called Y
  if (mesh->spatial_dimension() != 2) {
    std::__throw_invalid_argument(
        "You are calling closestPair2D for a mesh which is not 2D!");
  }

  std::vector<libMesh::Node *> Y = X;

  // Sort Y by the y coordinate
  sortByIthCoord(Y_AXIS, Y);

  int n = X.size();

  // Base cases
  if (n == 2) {
    return eucDist(X[0], X[1]);
  }
  if (n == 3) {
    std::vector<double> dists{eucDist(X[0], X[1]), eucDist(X[0], X[2]),
                              eucDist(X[1], X[2])};
    return *std::min_element(dists.begin(), dists.end());
  }

  std::vector<libMesh::Node *> xSideOne(X.begin(), X.begin() + n / 2);
  std::vector<libMesh::Node *> xSideTwo(X.begin() + n / 2, X.end());

  double midX = (**(X.begin() + n / 2))(0);

  // Recursion
  double d1 = closestPair2D(xSideOne);
  double d2 = closestPair2D(xSideTwo);

  double delta = std::min(d1, d2);

  std::vector<libMesh::Node *> S;
  for (auto &node : Y) {
    if ((midX - delta) <= (*node)(0) <= (midX + delta)) {
      S.push_back(node);
    }
  }

  for (int node_num = 0; node_num < S.size(); node_num++) {
    for (int i = 0; i < 7 && (S[node_num + i] != S.back()); i++) {
      eucDist(S[node_num], S[node_num + (i + 1)]) < delta
          ? delta = eucDist(S[node_num], S[node_num + (i + 1)])
          : delta = delta;
    }
  }
  return delta;
}

double ClosestPairFinder::closestPair3D(std::vector<libMesh::Node *> &X) {
  if (mesh->spatial_dimension() != 3) {
    std::__throw_invalid_argument(
        "You are calling closestPair3D for a mesh which is not 3D!");
  }
  // Create a copy of X called Y
  std::vector<libMesh::Node *> Y = X;

  // Sort Y by the y coordinate
  sortByIthCoord(Y_AXIS, Y);

  int n = X.size();
  // Base cases
  // if(n == 1){;}
  if (n == 2) {
    return eucDist(X[0], X[1]);
  }
  if (n == 3) {
    std::vector<double> dists{eucDist(X[0], X[1]), eucDist(X[0], X[2]),
                              eucDist(X[1], X[2])};
    return *std::min_element(dists.begin(), dists.end());
  }

  // std::vector<libMesh::Node*>::iterator midPoint = X[n/2];

  // Split X into two halves
  std::vector<libMesh::Node *> xSideOne(X.begin(), X.begin() + n / 2);
  std::vector<libMesh::Node *> xSideTwo(X.begin() + n / 2, X.end());

  // Have to dereference twice, once for the iterator and once for the node
  // pointer
  double midX = (**(X.begin() + n / 2))(0);

  // Recursion
  double minDistOne = closestPair3D(xSideOne);
  double minDistTwo = closestPair3D(xSideTwo);

  double delta = std::min(minDistOne, minDistTwo);

  std::vector<libMesh::Node *> slabSet;

  for (auto &node : Y) {
    if ((midX - delta) <= (*node)(0) <= (midX + delta)) {
      slabSet.push_back(node);
    }
  }

  // Incase there is nothing in the strip
  if (slabSet.size() == 0 | slabSet.size() == 1) {
    return delta;
  }

  // Create vector containing pairs of nodes
  std::vector<nodePair> pairs;

  getPotentialPairs(slabSet, delta, pairs);

  // For potential pairs, test to see if they are closer than delta
  for (auto &pair : pairs) {
    (eucDist(pair.first, pair.second) < delta)
        ? delta = eucDist(pair.first, pair.second)
        : delta = delta;
  }
  return delta;
}

void ClosestPairFinder::getPotentialPairs(std::vector<libMesh::Node *> &X,
                                          double delta,
                                          std::vector<nodePair> &pairs) {
  // Create a copy of X called Y
  std::vector<libMesh::Node *> Y = X;

  // Sort Y by the y coordinate
  sortByIthCoord(AXIS::Y_AXIS, Y);

  int numNodes = X.size();
  // Base cases

  if (numNodes == 2) {
    if (eucDist(X[0], X[1]) <= delta) {
      pairs.push_back(nodePair(X[0], X[1]));
    }
    return;
  }

  if (numNodes == 3) {
    std::vector<double> dists{eucDist(X[0], X[1]), eucDist(X[0], X[2]),
                              eucDist(X[1], X[2])};
    if (eucDist(X[0], X[1]) <= delta) {
      pairs.push_back(nodePair(X[0], X[1]));
    }

    if (eucDist(X[0], X[2]) <= delta) {
      pairs.push_back(nodePair(X[0], X[2]));
    }

    if (eucDist(X[1], X[2]) <= delta) {
      pairs.push_back(nodePair(X[1], X[2]));
    }
    return;
  }

  std::vector<libMesh::Node *> xS1(X.begin(), X.begin() + numNodes / 2);
  std::vector<libMesh::Node *> xS2(X.begin() + numNodes / 2, X.end());

  double midX = (**(X.begin() + numNodes / 2))(0);
  // Recursion
  getPotentialPairs(xS1, delta, pairs);
  getPotentialPairs(xS2, delta, pairs);

  std::vector<libMesh::Node *> S;
  for (auto &node : Y) {
    if ((midX - delta) <= (*node)(0) <= (midX + delta)) {
      S.push_back(node);
    }
  }

  int max_node_comparisons = std::max(7, (int)S.size() - 1);

  for (int node_num = 0; node_num < S.size(); node_num++) {
    for (int i = 0; i < 7 && (S[node_num + i] != S.back()); i++) {
      if (eucDist(S[node_num], S[node_num + (i + 1)]) <= delta) {
        pairs.push_back(nodePair(S[node_num], S[node_num + (i + 1)]));
      }
    }
  }
}

void ClosestPairFinder::sortByIthCoord(AXIS axis,
                                       std::vector<libMesh::Node *> &vec) {
  // Throw an error if user does not give a good axis argument
  // if(axis != (AXIS::X_AXIS | AXIS::Y_AXIS | AXIS::Z_AXIS))
  // {
  //     std::cerr << "Must call sortByIthCoord with either X Y or Z as axis
  //     arg!" << std::endl; std::__throw_bad_function_call();
  // }
  std::sort(vec.begin(), vec.end(),
            [axis](libMesh::Node *lhs, libMesh::Node *rhs) {
              return (*lhs)(axis) < (*rhs)(axis);
            });
}

void ClosestPairFinder::initialiseArrays() {
  xPoints.reserve(mesh->n_nodes());
  // Put all nodes from mesh into xPoints vector
  for (auto node : mesh->node_ptr_range()) {
    xPoints.emplace_back(node);
  }
  sortByIthCoord(AXIS::X_AXIS, xPoints);
}

double ClosestPairFinder::eucDist(libMesh::Node *n1, libMesh::Node *n2) {
  return std::sqrt(std::pow(((*n2)(0) - (*n1)(0)), 2) +
                   std::pow(((*n2)(1) - (*n1)(1)), 2) +
                   std::pow(((*n2)(2) - (*n1)(2)), 2));
}

double ClosestPairFinder::closestPairMagnitude(int dim) {
  switch (dim) {
  case 2:
    return closestPair2D(xPoints);

  case 3:
    return closestPair3D(xPoints);
  }
  std::__throw_bad_function_call;
  return 0;
}
} // namespace VacuumMesher