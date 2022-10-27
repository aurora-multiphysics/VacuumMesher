#include "genFullMesh.hpp"


void 
getGeometryBoundaries(libMesh::Mesh& geometryMesh, std::vector<libMesh::dof_id_type>& node_id_list, std::vector<libMesh::boundary_id_type>& bc_id_list)
{
    geometryMesh.get_boundary_info().build_node_list_from_side_list();
    geometryMesh.get_boundary_info().build_node_list(node_id_list, bc_id_list);
}

void 
getGeometryConnectivity(libMesh::Mesh& geometryMesh,
                        std::vector<std::vector<unsigned int>>& connectivity)
{
    unsigned int elem_counter = 0;
    for(auto& elem: as_range(geometryMesh.local_elements_begin(), geometryMesh.local_elements_end()))
    {
        
        unsigned int node_counter = 0;
        for(auto& node: elem->node_ref_range())
        {
            connectivity[elem_counter][node_counter] = node.id();
            node_counter++;
        }
        elem_counter++;
    }
}

void 
findDuplicateVerts(libMesh::Mesh& vacuumMesh, libMesh::Mesh& geometryMesh, 
                     std::map<int, int>& geomToVacNodes, std::vector<unsigned int>& duplicateNodeIds)
{
    int matches = 0;
    for(auto& geomNode: geometryMesh.local_node_ptr_range())
    {
        for(auto& node: vacuumMesh.local_node_ptr_range())
        {
            if((*node) == (*geomNode))
            {
                matches ++;
                geomToVacNodes[geomNode->id()] = node->id();
                duplicateNodeIds.push_back(geomNode->id());
            }
        }
    }
}

void 
addGeomVerts(libMesh::Mesh& geometryMesh, 
            libMesh::Mesh& vacuumMesh, 
            std::map<int, int>& geomToVacNodes, 
            std::vector<unsigned int>& duplicateNodeIds)
{
    int counter = 0;
    for(auto& node: geometryMesh.local_node_ptr_range())
    {
        if(!std::binary_search(duplicateNodeIds.begin(), duplicateNodeIds.end(), node->id()))
        {
            int id = vacuumMesh.max_node_id();
            vacuumMesh.add_point(*node, id);
            geomToVacNodes[node->id()] = id;
        }
    }
}


void 
createFullGeometry(libMesh::Mesh& geometryMesh, libMesh::Mesh& vacuumMesh)
{
    std::cout << "Getting full geom" << std::endl;
    libMesh::ElemType elem_type, face_type; 
    int num_elem_faces, num_face_nodes;
    int num_nodes_per_el = 4;
    std::map<int, int> geomToVacNodes;
    std::vector<libMesh::dof_id_type> bdr_node_id_list;
    std::vector<libMesh::boundary_id_type> bc_id_list;
    std::vector<unsigned int> duplicateNodeIds;

    std::vector<std::vector<unsigned int>>connectivity(geometryMesh.n_local_elem(), std::vector<unsigned int>(num_nodes_per_el, 1));
    
    std::cout << "Getting elem ptr" << std::endl;
    libMesh::Elem* geom_elemPtr = geometryMesh.elem_ptr(1);
    // std::cout << geom_elemPtr->type() << std::endl;
    getElemInfo(elem_type, face_type, geom_elemPtr, num_elem_faces, num_face_nodes);
    std::cout << "Getting connectivity" << std::endl;
    getGeometryConnectivity(geometryMesh, connectivity);
    std::cout << "Getting boundaries" << std::endl;
    getGeometryBoundaries(geometryMesh, bdr_node_id_list, bc_id_list);
    std::cout << "Finding duplicate vertices" << std::endl;
    findDuplicateVerts(vacuumMesh, geometryMesh, geomToVacNodes, duplicateNodeIds);
    std::cout << "Adding new vertices" << std::endl;
    addGeomVerts(geometryMesh, vacuumMesh, geomToVacNodes, duplicateNodeIds);
    
    std::cout << "Building final mesh" << std::endl;
    int elem_count = 0;
    for(auto& elem_conn: connectivity)
    {
        libMesh::Elem* elem = libMesh::Elem::build(elem_type).release();
        for(int j = 0; j < vacuumMesh.elem_ref(0).n_nodes(); j++)
        {
            elem->set_node(j) = vacuumMesh.node_ptr(geomToVacNodes[connectivity[elem_count][j]]);
        }
        elem->set_id(1+vacuumMesh.max_elem_id());
        vacuumMesh.add_elem(elem);
        elem_count++;
    }
    std::cout << "Building sidesets" << std::endl;
    for(int node_num = 0; node_num < bdr_node_id_list.size(); node_num++)
    {
        vacuumMesh.boundary_info->add_node(geomToVacNodes[bdr_node_id_list[node_num]], bc_id_list[node_num]);
    }
    vacuumMesh.boundary_info->build_side_list_from_node_list();
    // vacuumMesh.get_boundary_info().print_info(std::cout);
    for(auto& entry: geometryMesh.get_boundary_info().get_sideset_name_map())
    {
        vacuumMesh.get_boundary_info().set_sideset_name_map()[entry.first] = entry.second;
    }
    vacuumMesh.prepare_for_use();
    vacuumMesh.write("fullGeom.e");
}


