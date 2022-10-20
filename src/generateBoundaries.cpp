#include "generateBoundaries.hpp"
#include "removeDuplicateVerts.hpp"

void 
getBoundingEdges(libMesh::Elem* elem, 
                 std::vector<libMesh::dof_id_type>& element_id_list,
                 std::vector<int>& boundarySides)
{
    for(int side = 0; side < elem->n_sides(); side++)
    {
        auto neighbor = elem->neighbor_ptr(side);
        if(!std::binary_search(element_id_list.begin(), element_id_list.end(), neighbor->id()))
        {
            boundarySides.push_back(side);
            // std::cout << "found" << std::endl;
        }
    }     
}

void
createEdgeMesh(libMesh::Mesh& surfaceMesh, libMesh::Mesh& boundaryMesh)
{

    unsigned int boundaryElemCount = 0;
    // std::vector<int> boundEdgeList;
    std::vector<libMesh::dof_id_type> element_id_list;
    std::vector<unsigned short int> side_list;
    std::vector<libMesh::boundary_id_type> bc_id_list;
    std::vector<libMesh::dof_id_type> current_node_ids;
    std::vector<int> boundarySides;
    std::vector<std::vector<int>> conn;

    std::map<int, int> newNodeIds;
    

    surfaceMesh.get_boundary_info().build_active_side_list(element_id_list, side_list, bc_id_list);
    //Get list of boundary elements
    std::sort(element_id_list.begin(), element_id_list.end());
    std::vector<libMesh::dof_id_type>::iterator newEnd;
    newEnd = std::unique(element_id_list.begin(), element_id_list.end());
    element_id_list.resize(std::distance(element_id_list.begin(), newEnd));

    for(libMesh::dof_id_type& elem_id: element_id_list)
    {
        std::vector<int> boundarySides;
        libMesh::Elem* elem = surfaceMesh.elem_ptr(elem_id);

        getBoundingEdges(elem, element_id_list, boundarySides);

        for(auto& side_num: boundarySides)
        {  
            std::vector<int> side_conn;
            std::unique_ptr<libMesh::Elem> edge = elem->side_ptr(side_num);
            for(auto& node: edge->node_ref_range())
            {
                current_node_ids.push_back(node.id());
                side_conn.push_back(node.id());
            }
            conn.push_back(side_conn);
            boundaryElemCount++;
        }
    }

    std::sort(current_node_ids.begin(), current_node_ids.end());
    newEnd = std::unique(current_node_ids.begin(), current_node_ids.end());
    current_node_ids.resize(std::distance(current_node_ids.begin(), newEnd));

    for(int i = 0; i < (int) current_node_ids.size(); i++)
    {
        newNodeIds[current_node_ids[i]] = i;
    }

    for(auto node: current_node_ids)
    {   
        libMesh::Node* node_ptr = surfaceMesh.node_ptr(node);
        double pnt[3];
        pnt[0] = (*node_ptr)(0);
        pnt[1] = (*node_ptr)(1);
        pnt[2] = (*node_ptr)(2);
        libMesh::Point xyz(pnt[0], pnt[1], pnt [2]);
        boundaryMesh.add_point(xyz, newNodeIds[node]);
        std::cout << pnt[0] << " " << pnt[1] << " " << pnt[2] << std::endl;
    }

    for(int elem_num = 0; elem_num < boundaryElemCount; elem_num++)
    {
        libMesh::Elem* new_edge = libMesh::Elem::build((libMesh::ElemType)0).release();
        for(int node_num = 0; node_num < 2; node_num++)
        {
            new_edge->set_node(node_num) = surfaceMesh.node_ptr(newNodeIds[conn[elem_num][node_num]]);
        }
        new_edge->set_id(elem_num);
        boundaryMesh.add_elem(new_edge);
    }
    std::cout << "Number of boundary edges = " << boundaryElemCount << std::endl;
    // boundaryMesh.renumber_nodes_and_elements();
    boundaryMesh.set_mesh_dimension(2); //Should this be 2 or 3???
    boundaryMesh.set_spatial_dimension(3);
    boundaryMesh.write("boundaryMeshTester.e");

    // Need H, set of 2D node positions
    // E, E * 2 array of edges 
}




