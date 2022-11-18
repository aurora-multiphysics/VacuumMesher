#include "removeDupeNodes.hpp"

// Global hits
std::vector<int> hits;

bool callback(int id) {  
  hits.push_back(id);
  return true;
}

void 
createTree(RTree<int, double, 3, float> &rtree, libMesh::Mesh& vacMesh, double& tol)
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
searchTree(RTree<int, double, 3, float> &rtree, double& tol, libMesh::Mesh& vacMesh, libMesh::Node* node)
{
    std::array node_coords = {(*node)(0), (*node)(1), (*node)(2)};
    Box node_box = Box(node_coords, tol, node->id());
    // Empty global hits vector so it doesn't contain results of previous search
    hits.clear();
    // Search tree to see if thie node already exists 
    int nhits = rtree.Search(node_box.min.data(), node_box.max.data(), callback);
    if(!nhits)
    {
        return true;
    }
    else
    {   
        for(auto& hit: hits)
        {
            for(int i = 0; i<3;i++)
            {
                if(((*node)(i) - (*(vacMesh.node_ptr(hit)))(i)) > 1e-02)
                {
                    std::cout << "BAD" << std::endl;
                }
            }
        }
        return false;
    }
    return true;
}



void 
combineMesh(RTree<int, double, 3, float> &rtree, double& tol, libMesh::Mesh& surfMesh, libMesh::Mesh& vacMesh)
{
    for(auto& node: surfMesh.local_node_ptr_range())
    {
        if(!searchTree(rtree, tol, vacMesh, node))
        {
        }
    }
}
