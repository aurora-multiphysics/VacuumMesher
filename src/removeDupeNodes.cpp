#include "removeDupeNodes.hpp"

// Global hits
std::vector<int> hits;

// callback function for rtree
bool callback(int id) {  
  hits.push_back(id);
  return true;
}

void 
createTree(RTree<int, double, 3, float> &rtree, libMesh::Mesh& vacMesh, const double& tol)
{
    // Add all existing verts to rTree
    for(auto& node: vacMesh.local_node_ptr_range())
    {
        std::array node_coords = {(*node)(0), (*node)(1), (*node)(2)};
        Box node_box = Box(node_coords, tol, node->id());
        insertNode(rtree, node_box);
    }
}

void 
insertNode(RTree<int, double, 3, float> &rtree, Box& node_box)
{
    // Check if node exists in rTree, if not, insert, if it does, make sure connectivity still works
    rtree.Insert(node_box.min.data(), node_box.max.data(), node_box.node_id);
}

bool
searchTree(RTree<int, double, 3, float> &rtree, 
           const double& tol,
           std::map<unsigned int, unsigned int>& id_map,
           libMesh::Mesh& vacMesh,
           libMesh::Node* node)
{
    // Empty global hits vector so it doesn't contain results of previous search
    hits.clear();
    // 
    int node_indent = vacMesh.n_nodes();
    // 
    std::array node_coords = {(*node)(0), (*node)(1), (*node)(2)};
    Box node_box = Box(node_coords, tol, node->id());
    
    
    // Search tree to see if thie node already exists 
    int nhits = rtree.Search(node_box.min.data(), node_box.max.data(), callback);
    if(!nhits)
    {
        id_map[node->id()] = node_indent + 1;
        return true;
    }
    else if(nhits == 1)
    {   
        id_map[node->id()] = hits[0];
        return false;        
    }
    return true;
}

void 
combineMesh(const double& tol,
            libMesh::Mesh& surfMesh, 
            libMesh::Mesh& vacMesh,
            std::multimap<unsigned int, unsigned int> surfaceFaceMap)
{
    // Instantiate an rTree. Using a rTree data structure significantly reduces the amount of
    // time taken to discover duplicate nodes.
    RTree<int, double, 3, float> rtree;

            else if(abs((*node_1)(0) == (*node_2)(0)) < 1e-8 &&
                    abs((*node_1)(1) == (*node_2)(1)) < 1e-8 &&
                    abs((*node_1)(2) - (*node_2)(2)) < 1e-8)
            {           
                libMesh::dof_id_type old_id = id_1 > id_2 ? id_1 : id_2;
                libMesh::dof_id_type new_id = id_1 < id_2 ? id_1 : id_2;

    // This is a map which helps us keep track of the node id's of the duplicate nodes. For example,
    // if Node 4 in the surface mesh and Node 6 in the vacuum mesh are the same, "id_map[4]" will return 6
    std::map<unsigned int, unsigned int> id_map;
    
    // Loop over all the nodes in the original geometry. If the node is not a duplicate, add it to the mesh
    // , if the node is a duplicate, it is  
    for(auto& node: surfMesh.local_node_ptr_range())
    {
        // If there are no matches in the vacuumMesh, then we add the point 
        if(searchTree(rtree, tol, id_map, vacMesh, node))
        {
            double pnt[3];
            pnt[0] = (*node)(0);
            pnt[1] = (*node)(1);
            pnt[2] = (*node)(2);
            libMesh::Point xyz(pnt[0], pnt[1], pnt[2]);
            vacMesh.add_point(xyz, id_map[node->id()]);
        }
    }
    
    // Loop adds all elements from original geometry to vacuum mesh. The id_map is used to make sure the correct connectivity
    // is used where there are duplicate nodes
    for(auto& elem: libMesh::as_range(surfMesh.local_elements_begin(), surfMesh.local_elements_end()))
    {
        unsigned int el_id = 1 + vacMesh.n_elem(); 
        libMesh::Elem* new_elem = libMesh::Elem::build(libMesh::TET4).release();
        for(int j = 0; j < new_elem->n_nodes(); j++)
        {
            new_elem->set_node(j) = vacMesh.node_ptr(id_map[elem->node_ptr(j)->id()]);
        }
        new_elem->set_id(el_id);
        new_elem->subdomain_id() = 2;
        vacMesh.add_elem(new_elem);

        // Makes the boundary between the original geometry and the vacuum mesh a sideset in the mesh,
        // and names it "vacuum_boundary"
        for(auto& boundSide: libMesh::as_range(surfaceFaceMap.equal_range(elem->id())))
        {
            vacMesh.get_boundary_info().add_side(el_id, boundSide.second, 1);
        }
        // Set boundary name!
    }
    // Prepare the mesh for use. This libmesh method does some id renumbering etc, generally a good idea
    // to call it after constructing a mesh
    vacMesh.prepare_for_use();
}
