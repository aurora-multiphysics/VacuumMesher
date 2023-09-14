#include "Tetrahedralisation/removeDupeNodes.hpp"

// Global hits
std::vector<int> hits;

// callback function for rtree
bool callback(int id) {
  hits.push_back(id);
  return true;
}

void createTree(nodeTree &rtree, Eigen::MatrixXd &V, const double &tol) {
  // Add all existing verts to rTree
  for (int node_id = 0; node_id < V.rows(); node_id++) {
    std::array node_coords = {(V.row(node_id))(0), (V.row(node_id))(1),
                              (V.row(node_id)(2))};
    Box node_box = Box(node_coords, tol, node_id);
    insertNode(rtree, node_box);
  }
}

void createTree(nodeTree &rtree, libMesh::Mesh &meshOne, const double &tol) {
  // Add all existing verts to rTree
  for (auto &node : meshOne.local_node_ptr_range()) {
    std::array node_coords = {(*node)(0), (*node)(1), (*node)(2)};
    Box node_box = Box(node_coords, tol, node->id());
    insertNode(rtree, node_box);
  }
}

void insertNode(nodeTree &rtree, Box &node_box) {
  // Check if node exists in rTree, if not, insert, if it does, make sure
  // connectivity still works
  rtree.Insert(node_box.min.data(), node_box.max.data(), node_box.node_id);
}

bool searchTree(nodeTree &rtree, const double &tol,
                std::map<unsigned int, unsigned int> &id_map, int node_indent,
                Box &node_box) {
  // Empty global hits vector so it doesn't contain results of previous search
  hits.clear();
  //
  // int node_indent = meshOne.n_nodes();
  //

  // Search tree to see if thie node already exists
  int nhits = rtree.Search(node_box.min.data(), node_box.max.data(), callback);
  if (!nhits) {
    // id_map[node_box.node_id] = node_indent; //+ 1;
    insertNode(rtree, node_box);
    return true;
  } else if (nhits == 1) {
    return false;
  } else {
    std::cout
        << "Duplicate nodes may already exist in this mesh prior to combining!"
        << std::endl;
  }

  return true;
}

void combineMeshes(const double &tol, libMesh::Mesh &meshOne,
                   libMesh::Mesh &meshTwo,
                   std::multimap<unsigned int, unsigned int> surfaceFaceMap) {
  std::set<libMesh::subdomain_id_type> sub_ids;
  meshOne.subdomain_ids(sub_ids);
  libMesh::subdomain_id_type vacId = 2;
  if (!sub_ids.empty()) {
    vacId = 1 + (*(sub_ids.rbegin()));
    std::cout << "vac Id will be " << vacId << std::endl;
  }

  // Instantiate an rTree. Using a rTree data structure significantly reduces
  // the amount of time taken to discover duplicate nodes.
  nodeTree rtree;

  // Generate the initial tree, containing all the nodes of the vacuum mesh. We
  // will
  //  check the nodes of the part mesh against this tree to search for duplicate
  //  nodes
  createTree(rtree, meshOne, tol);

  // This is a map which helps us keep track of the node id's of the duplicate
  // nodes. For example, if Node 4 in the surface mesh and Node 6 in the vacuum
  // mesh are the same, "id_map[4]" will return 6
  std::map<unsigned int, unsigned int> id_map;
  for (auto &node : meshTwo.local_node_ptr_range()) {
    // Create box representing the node to search rTree with
    std::array node_coords = {(*node)(0), (*node)(1), (*node)(2)};
    Box node_box = Box(node_coords, tol, node->id());
    // Get number of nodes currently in the mesh, so if we have to add a node
    //  we know what id to give it (node_indent + 1)
    int node_offset = meshOne.n_nodes();
    // If there are no matches in the vacuumMesh, then we add the point
    if (!searchTree(rtree, tol, id_map, node_offset, node_box)) {
      // If there are no matches in the vacuumMesh, then we add the point
      id_map[node->id()] = hits[0];
      continue;
    }
    //
    id_map[node->id()] = node_offset;

    double pnt[3];
    pnt[0] = (*node)(0);
    pnt[1] = (*node)(1);
    pnt[2] = (*node)(2);
    libMesh::Point xyz(pnt[0], pnt[1], pnt[2]);
    meshOne.add_point(xyz, id_map[node->id()]);
  }

  // Loop adds all elements from original geometry to vacuum mesh. The id_map is
  // used to make sure the correct connectivity is used where there are
  // duplicate nodes
  for (auto &elem : libMesh::as_range(meshTwo.local_elements_begin(),
                                      meshTwo.local_elements_end())) {
    unsigned int el_id = 1 + meshOne.n_elem();
    // Element type being build must be consistent between meshes
    libMesh::Elem *new_elem =
        libMesh::Elem::build(meshOne.elem_ptr(0)->type()).release();
    for (int j = 0; j < new_elem->n_nodes(); j++) {
      new_elem->set_node(j) = meshOne.node_ptr(id_map[elem->node_ptr(j)->id()]);
    }
    new_elem->set_id(el_id);
    new_elem->subdomain_id() = vacId;
    meshOne.add_elem(new_elem);

    // Makes the boundary between the original geometry and the vacuum mesh a
    // sideset in the mesh, and names it "vacuum_boundary"

    // Set boundary name!
  }

  for (auto boundSide = surfaceFaceMap.begin();
       boundSide != surfaceFaceMap.end(); boundSide++) {
    // std::cout << boundSide->first << std::endl;
    meshOne.get_boundary_info().add_side(boundSide->first, boundSide->second,
                                         5);
  }
  meshOne.boundary_info->set_sideset_name_map()[5] = "part_boundary";
  meshOne.subdomain_name(vacId) = "vacuum_region";
  // Prepare the mesh for use. This libmesh method does some id renumbering etc,
  // generally a good idea to call it after constructing a mesh
  meshOne.prepare_for_use();
}

// Version of the method that doesn't require a surfaceFaceMap, just combines
// the meshes
void combineMeshes(const double &tol, libMesh::Mesh &meshOne,
                   libMesh::Mesh &meshTwo) {
  // Instantiate an rTree. Using a rTree data structure significantly reduces
  // the amount of time taken to discover duplicate nodes.
  nodeTree rtree;

  // Generate the initial tree, containing all the nodes of the vacuum mesh. We
  // will
  //  check the nodes of the part mesh against this tree to search for duplicate
  //  nodes
  createTree(rtree, meshOne, tol);

  // This is a map which helps us keep track of the node id's of the duplicate
  // nodes. For example, if Node 4 in the surface mesh and Node 6 in the vacuum
  // mesh are the same, "id_map[4]" will return 6
  std::map<unsigned int, unsigned int> id_map;

  // Loop over all the nodes in the original geometry. If the node is not a
  // duplicate, add it to the mesh , if the node is a duplicate, it is
  for (auto &node : meshTwo.local_node_ptr_range()) {
    // Create box representing the node to search rTree with
    std::array node_coords = {(*node)(0), (*node)(1), (*node)(2)};
    Box node_box = Box(node_coords, tol, node->id());
    // Update node count and henceforth the offset for numbering new nodes
    int node_offset = meshOne.n_nodes();

    if (!searchTree(rtree, tol, id_map, node_offset, node_box)) {
      // If there are no matches in the vacuumMesh, then we add the point
      id_map[node->id()] = hits[0];
      continue;
    }
    //
    id_map[node->id()] = node_offset;

    double pnt[3];
    pnt[0] = (*node)(0);
    pnt[1] = (*node)(1);
    pnt[2] = (*node)(2);
    libMesh::Point xyz(pnt[0], pnt[1], pnt[2]);
    meshOne.add_point(xyz, id_map[node->id()]);
  }

  // Loop adds all elements from original geometry to vacuum mesh. The id_map is
  // used to make sure the correct connectivity is used where there are
  // duplicate nodes
  for (auto &elem : libMesh::as_range(meshTwo.local_elements_begin(),
                                      meshTwo.local_elements_end())) {
    unsigned int el_id = 1 + meshOne.n_elem();
    // Element type being build must be consistent between meshes
    libMesh::Elem *new_elem =
        libMesh::Elem::build(meshOne.elem_ptr(0)->type()).release();
    for (int j = 0; j < new_elem->n_nodes(); j++) {
      new_elem->set_node(j) = meshOne.node_ptr(id_map[elem->node_ptr(j)->id()]);
    }
    new_elem->set_id(el_id);
    // new_elem->subdomain_id() = 2;
    meshOne.add_elem(new_elem);
  }
  // Prepare the mesh for use. This libmesh method does some id renumbering etc,
  // generally a good idea to call it after constructing a mesh
  meshOne.prepare_for_use();
}

void combineMeshes(const double &tol, Eigen::MatrixXd &V1, Eigen::MatrixXi &F1,
                   Eigen::MatrixXd &V2, Eigen::MatrixXi &F2) {
  // Instantiate an rTree. Using a rTree data structure significantly reduces
  // the amount of time taken to discover duplicate nodes.
  nodeTree rtree;

  // This is a map which helps us keep track of the node id's of the duplicate
  // nodes. For example, if Node 4 in the surface mesh and Node 6 in the vacuum
  // mesh are the same, "id_map[4]" will return 6
  std::map<unsigned int, unsigned int> id_map;

  createTree(rtree, V1, tol);

  int starting_elems = F1.rows();
  int node_indent = V1.rows();

  for (int node_id = 0; node_id < V2.rows(); node_id++) {
    Box node_box(V2.row(node_id), tol, node_indent);
    // If there are no matches in the vacuumMesh, then we add the point
    if (!searchTree(rtree, tol, id_map, node_indent, node_box)) {
      id_map[node_id] = hits[0];
      continue;
    }

    id_map[node_id] = node_indent;
    node_indent++;
  }

  V1.conservativeResize(node_indent, 3);

  nodeTree::Iterator it;
  for (rtree.GetFirst(it); !rtree.IsNull(it); rtree.GetNext(it)) {
    double boundsMin[3] = {0., 0., 0.};
    double boundsMax[3] = {0., 0., 0.};
    it.GetBounds(boundsMin, boundsMax);

    double centre[3];
    for (int i = 0; i < 3; i++) {
      centre[0] = boundsMin[0] + ((boundsMax[0] - boundsMin[0]) / 2);
      centre[1] = boundsMin[1] + ((boundsMax[1] - boundsMin[1]) / 2);
      centre[2] = boundsMin[2] + ((boundsMax[2] - boundsMin[2]) / 2);
    }
    V1.row (*it)(0) = centre[0];
    V1.row (*it)(1) = centre[1];
    V1.row (*it)(2) = centre[2];
  }

  F1.conservativeResize(F1.rows() + F2.rows(), 3);

  for (int elem_id = 0; elem_id < F2.rows(); elem_id++) {
    F1.row(elem_id + starting_elems)(0) = id_map[F2.row(elem_id)(0)];
    F1.row(elem_id + starting_elems)(1) = id_map[F2.row(elem_id)(1)];
    F1.row(elem_id + starting_elems)(2) = id_map[F2.row(elem_id)(2)];
  }
}
