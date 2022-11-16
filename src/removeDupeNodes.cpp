#include "removeDupeNodes.hpp"

int 
createTree(RTree<int, double, 3, float> rtree, libMesh::Mesh& mesh, double tol)
{
    // Add all existing verts to rTree
    int count = 0;
    for(auto& node: mesh.local_node_ptr_range())
    {
        std::cout << node->id() << std::endl;
        insertNode(rtree, *node, count);
        count ++;
    }
    return count;
}

bool
searchTree(RTree<int, double, 3, float> rtree, libMesh::Node& node)
{
    // Search tree to see if thie node already exists 
    return true;
}

void 
insertNode(RTree<int, double, 3, float> rtree, libMesh::Node& node, int count)
{
    // Check if node exists in rTree, if not, insert, if it does, make sure connectivity still works
    std::array node_coords = {(node)(0), (node)(1), (node)(2)};
    Box new_node = Box(node_coords, 1e-06, node.id());
    rtree.Insert(new_node.min.data(), new_node.max.data(), count);
}
