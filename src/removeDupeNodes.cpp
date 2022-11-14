#include "removeDupeNodes.hpp"


void 
removeDupeNodes(libMesh::Mesh& mesh)
{
    std::cout << "Removing duplicate nodes" << std::endl;
    std::vector<libMesh::dof_id_type> old_ids;
    std::vector<std::vector<libMesh::dof_id_type>> nodes_to_elem_map;
    libMesh::MeshTools::build_nodes_to_elem_map(mesh, nodes_to_elem_map);

    for(auto& node_1 :mesh.node_ptr_range())
    {
        libMesh::dof_id_type id_1 = node_1->id();
        
        for(auto& node_2: mesh.node_ptr_range())
        {
            libMesh::dof_id_type id_2 = node_2->id();
    
            if(id_1==id_2)
            {
                continue;
            }

            else if(id_1 > id_2)
            {
                continue;
            }

            else if(((*node_1)(0) == (*node_2)(0)) &&
                    ((*node_1)(1) == (*node_2)(1)) &&
                    abs((*node_1)(2) - (*node_2)(2)) < 1e-8)
            {           
                libMesh::dof_id_type old_id = id_1 > id_2 ? id_1 : id_2;
                libMesh::dof_id_type new_id = id_1 < id_2 ? id_1 : id_2;

                std::cout << "Found match, " << old_id << ", " << new_id << std::endl;

                for(auto& elem: nodes_to_elem_map[old_id])
                {
                    libMesh::Elem* elem_ptr = mesh.elem_ptr(elem);
                    elem_ptr->set_node(elem_ptr->local_node(old_id)) = mesh.node_ptr(new_id);
                } 
                mesh.delete_node(mesh.node_ptr(old_id));
            }
        }
    }
    mesh.prepare_for_use();
    std::cout << "Dupes removed" << std::endl;
}