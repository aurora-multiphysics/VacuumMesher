#include "SurfaceMeshing/SurfaceGenerator.hpp"
#include <algorithm>

SurfaceMeshGenerator::~SurfaceMeshGenerator() {
  // DESTROY
}

SurfaceMeshGenerator::SurfaceMeshGenerator(
    libMesh::Mesh &meshRef, libMesh::Mesh &surfaceMeshRef,
    std::vector<libMesh::Elem *> elements)
    : mesh(meshRef), surfaceMesh(surfaceMeshRef), el_set({}) {

  // Retrieve useful element info
  getElemInfo(elem_type, face_type, mesh.elem_ptr(0), num_elem_sides,
              num_face_nodes);

  // Checks to see if user has requested only a subset of the elements to be
  // skinned, if they have, the element id's they have supplied are stored in a
  // set. Using a set instead of the supplied vector will remove any duplicate
  // element id's.

  // If the user wants to skin the whole mesh, they don't have to provide
  // anything. A set containing all the element id's in the mesh is created for
  // them
  if (!elements.empty()) {
    this->el_set = elements;
  } else {
    for (auto &elem : mesh.element_ptr_range()) {
      el_set.emplace_back(elem);
    }
  }
}

// isElementSurface for if user wants the whole mesh skinned
void SurfaceMeshGenerator::isElementSurface(libMesh::Elem *element,
                                            std::vector<int> &surface_faces) {
  int neighbor_counter = 0;
  for (int side = 0; side < element->n_sides(); side++) {
    if (element->neighbor_ptr(side) == nullptr) {
      surface_faces[neighbor_counter++] = side;
    }
  }
}

// isElementSurface for if user specifies only a subset of elements to be
// "skinned"
void SurfaceMeshGenerator::isElementSurface(
    libMesh::Elem *element, std::vector<libMesh::Elem *> &el_set,
    std::vector<int> &surface_faces) {

  // Counter to store number of neighbors found
  int neighbor_counter = 0;

  //  Loop over all sides in an element
  for (int side = 0; side < element->n_sides(); side++) {
    if (element->neighbor_ptr(side) == nullptr) {
      surface_faces[neighbor_counter++] = side;
    } else {
      if (!std::binary_search(el_set.begin(), el_set.end(),
                              element->neighbor_ptr(side))) {
        surface_faces[neighbor_counter++] = side;
      }
    }
  }
}

// Get surface for when the user just wants the whole mesh skinned, and not a
// subsection of it
//  i.e. they haven't specified an el_set
void SurfaceMeshGenerator::getSurface(bool writeMesh,
                                      std::string output_filename) {

  // Sideset info
  std::map<int, std::vector<libMesh::boundary_id_type>> boundary_data;

  // Map from old node ids to the new ones in the surface mesh
  std::vector<int> original_node_ids;
  std::map<int, int> surface_node_ids;

  // Connectivity of all the elements in the surface mesh
  std::vector<int> connectivity;

  // Loops over all the elements in the input vector
  for (auto &&elem : el_set) {

    // Instantiate vector to store sides that belong to the surface.
    //  Initialise all elements as -1. A value of -1 indicates end of list.
    //  This seems a bit more efficient that changing the size of the vector
    //  everytime a new surface face is found
    std::vector<int> surface_faces(num_elem_sides, -1);

    // Method to check whether the current element has faces that are on the
    // surface. If they exist, the face ID's are stored in the  in the
    // surface_faces vector
    if (el_set.size() < mesh.n_elem()) {
      isElementSurface(elem, el_set, surface_faces);
    } else {
      isElementSurface(elem, surface_faces);
    }

    // Loop over all of the sides found that belong to the boundary
    for (int &face_id : surface_faces) {

      if (face_id == -1) {
        break;
      }

      getFaceInfo(elem, face_id, original_node_ids, connectivity,
                  boundary_data);
    }
  }

  // Sorts the node ids in the original_node_ids in numerical order and removes
  // duplicates
  std::sort(original_node_ids.begin(), original_node_ids.end());
  std::vector<int>::iterator new_end;
  new_end = std::unique(original_node_ids.begin(), original_node_ids.end());
  original_node_ids.resize(std::distance(original_node_ids.begin(), new_end));

  // Map from the original_node_ids to the new node ids in the surface mesh
  for (int i = 0; i < (int)original_node_ids.size(); i++) {
    surface_node_ids[original_node_ids[i]] = i;
  }

  // Using the surface_node_ids map, set all the node data needed for the new
  // mesh
  surfaceMesh.reserve_nodes(original_node_ids.size());

  // Create nodes for our shiny new surface mesh
  for (auto node_id : original_node_ids) {
    libMesh::Node *node = mesh.node_ptr(node_id);
    double pnt[3];
    for (u_int i = 0; i < 3; i++) {
      pnt[i] = (*node)(i);
    }
    libMesh::Point xyz(pnt[0], pnt[1], pnt[2]);
    surfaceMesh.add_point(xyz, surface_node_ids[node_id]);
  }

  // Reserve elements for our shiny new surface mesh. There isn't much of a
  // performance advantage,
  //  but seen as we already know how many elements the surface mesh will have
  //  we might as well
  surfaceMesh.reserve_elem(surface_elem_counter);

  // For all of the surface elements, create the representitive 2D libmesh
  // element Connectivity is set and the element is added to the new mesh
  for (int i = 0; i < surface_elem_counter; i++) {
    libMesh::Elem *elem = libMesh::Elem::build(face_type).release();
    for (int j = 0; j < num_face_nodes; j++) {
      elem->set_node(j) = surfaceMesh.node_ptr(
          surface_node_ids[connectivity[(i * num_face_nodes) + j]]);
    }
    elem->set_id(i);
    elem->subdomain_id() = 1;
    surfaceMesh.add_elem(elem);
    for (auto &id : boundary_data[i]) {
      surfaceMesh.boundary_info->add_side(i, 0, id);
      surfaceMesh.boundary_info->sideset_name(id) =
          mesh.boundary_info->sideset_name(id);
    }
  }

  // Prepare mesh for use
  surfaceMesh.prepare_for_use();

  if (writeMesh) {
    surfaceMesh.write(output_filename);
  }
}

void SurfaceMeshGenerator::groupElems(
    libMesh::Mesh mesh, std::vector<std::vector<libMesh::dof_id_type>> &groups,
    NEIGHBOURTYPE neighbour_type) {

  std::set<int> elems;
  // Add all the element id's in the mesh to the "elems" set
  for (auto elem : mesh.element_ptr_range()) {
    elems.insert(elem->id());
  }

  // As long as there are element id's in the "elems" set, keep looping
  while (!elems.empty()) {

    auto it = elems.begin();
    libMesh::dof_id_type first_elem = *(it);
    elems.erase(it);
    std::set<libMesh::dof_id_type> neighbors;

    // Add the first element id to neighbors
    neighbors.insert(first_elem);

    // A vector to store element ID's that make up a group
    std::vector<libMesh::dof_id_type> groupElems(0);

    // While elements exist in neighbors
    while (!neighbors.empty()) {

      std::set<libMesh::dof_id_type> new_neighbors;

      // For all entries in neighbors
      for (auto &next_elem : neighbors) {

        // Add element ID in neighbors to groupElems vector
        groupElems.push_back(next_elem);

        // Get the libmesh element that corresponds to the current ID
        libMesh::Elem &elem = mesh.elem_ref(next_elem);

        std::set<const libMesh::Elem *> neighbors;

        // Are we looking for face neighbours (most common), vertex neighbors or
        // edge neighbors?
        switch (neighbour_type) {
        case (FACE):
          neighbors =
              std::set<const libMesh::Elem *>(elem.neighbor_ptr_range().begin(),
                                              elem.neighbor_ptr_range().end());
          break;

        case (VERTEX):

          elem.find_point_neighbors(neighbors);
          break;

        case (EDGE):
          elem.find_edge_neighbors(neighbors);
          break;
        }

        // Loop over neighbours
        for (auto &nnptr : neighbors) {

          // Pointer to next neighbour
          // const libMesh::Elem *nnptr = elem.neighbor_ptr(i);

          // If on boundary, a neighbour will be a nullptr
          if (nnptr == nullptr) {
            continue;
          }
          // Store id of the neighbor we just got
          libMesh::dof_id_type idnn = nnptr->id();

          // if the element id "idnn" still exists within elems
          if (elems.find(idnn) != elems.end()) {

            // Add idnn's neighbour ids to new_neighbors
            new_neighbors.insert(idnn);

            // Remove element idnn from elems
            elems.erase(idnn);
          }
        }
      }
      // End loop over previous neighbors

      // Found all the new neighbours, done with current set.
      neighbors = new_neighbors;
    }
    groups.push_back(groupElems);
  }
}

void SurfaceMeshGenerator::saveGroupedElems(
    libMesh::LibMeshInit &init, libMesh::Mesh &surfaceMesh,
    std::vector<std::vector<libMesh::dof_id_type>> &groups,
    std::string componentFilename) {
  unsigned int count = 0;

  if (groups.size() == 1) {
    std::cout
        << "All elements belong to one entity already, no grouping necessary"
        << std::endl;
    return;
  }

  for (auto &vector : groups) {
    // Mesh of subdomain we are going to save
    libMesh::Mesh newMesh(init.comm());
    libMesh::ElemType face_type;
    unsigned int num_elems, num_face_nodes;
    newMesh.clear();
    face_type = (surfaceMesh.elem_ref(0)).type();
    num_elems = vector.size();
    std::vector<unsigned int> current_nodes_ids;
    std::vector<unsigned int> connectivity;
    num_face_nodes = (surfaceMesh.elem_ref(0)).n_nodes();
    current_nodes_ids.reserve(num_elems * num_face_nodes);
    connectivity.reserve(num_elems * num_face_nodes);

    for (auto &elementID : vector) {
      libMesh::Elem &elem = surfaceMesh.elem_ref(elementID);
      for (auto &node : elem.node_ref_range()) {
        current_nodes_ids.emplace_back(node.id());
        connectivity.emplace_back(node.id());
      }
    }

    std::sort(current_nodes_ids.begin(), current_nodes_ids.end());
    std::vector<unsigned int>::iterator new_end;
    new_end = std::unique(current_nodes_ids.begin(), current_nodes_ids.end());
    current_nodes_ids.resize(std::distance(current_nodes_ids.begin(), new_end));
    std::map<unsigned int, unsigned int> nodeMap;

    for (int i = 0; i < current_nodes_ids.size(); i++) {
      nodeMap[current_nodes_ids[i]] = i;
    }

    for (auto &node_id : current_nodes_ids) {
      libMesh::Node *node = surfaceMesh.node_ptr(node_id);
      double pnt[3];
      pnt[0] = (*node)(0);
      pnt[1] = (*node)(1);
      pnt[2] = (*node)(2);
      libMesh::Point xyz(pnt[0], pnt[1], pnt[2]);
      newMesh.add_point(xyz, nodeMap[node_id]);
    }

    // For all of the surface elements, create the representitive 2D libmesh
    // element Connectivity is set and the element is added to the new mesh
    for (int i = 0; i < num_elems; i++) {
      // Create elem to add
      libMesh::Elem *elem = libMesh::Elem::build(face_type).release();

      for (int j = 0; j < num_face_nodes; j++) {
        elem->set_node(j) =
            newMesh.node_ptr(nodeMap[connectivity[(i * num_face_nodes) + j]]);
      }
      elem->set_id(i);
      newMesh.add_elem(elem);
    }
    newMesh.set_mesh_dimension(2); // Should this be 2 or 3???
    newMesh.set_spatial_dimension(3);
    newMesh.prepare_for_use();
    std::string newMeshName = componentFilename + std::to_string(count) + ".e";
    newMesh.write(newMeshName);

    count++;
  }
}

void SurfaceMeshGenerator::getFaceInfo(
    libMesh::Elem *elem, int &face_id, std::vector<int> &original_node_ids,
    std::vector<int> &connectivity,
    std::map<int, std::vector<libMesh::boundary_id_type>> &boundary_data) {
  // Check whether this side is a member of a sideset(s).
  // If yes, get the id of that sideset(s).
  std::vector<libMesh::boundary_id_type> boundary_ids;
  mesh.get_boundary_info().boundary_ids(elem, face_id, boundary_ids);

  if (boundary_ids.size() > 0) {
    boundary_data[surface_elem_counter] = boundary_ids;
  }

  // Loop over all the nodes on side 'surface_faces[i]'.
  // Add necessary information to containers for creating the mesh.
  for (auto &side_node : elem->build_side_ptr(face_id)->node_ref_range()) {

    // Get the global node id of the node in the original mesh
    int global_node_id = side_node.id();

    // Keep connectivity and node numbering data for constructing mesh later
    connectivity.push_back(global_node_id);
    original_node_ids.push_back(global_node_id);
  }

  // Keep a map from element id's to the face id's on that element that are part
  // of the surface mesh This will be helpful later
  surface_face_map.insert(std::make_pair(elem->id(), face_id));

  // Counter
  surface_elem_counter++;
}