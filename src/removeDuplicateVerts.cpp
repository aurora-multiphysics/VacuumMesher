#include "removeDuplicateVerts.hpp"

void 
getGeometryBoundaries(libMesh::Mesh& geometryMesh, std::vector<libMesh::dof_id_type>& node_id_list, std::vector<libMesh::boundary_id_type>& bc_id_list)
{
    geometryMesh.get_boundary_info().build_node_list_from_side_list();
    geometryMesh.get_boundary_info().build_node_list(node_id_list, bc_id_list);
}

void 
getGeometryConnectivity(libMesh::Mesh& geometryMesh, std::vector<std::vector<unsigned int>> connectivity)
{
    unsigned int elem_counter = 0;
    for(auto& elem: as_range(geometryMesh.local_elements_begin(), geometryMesh.local_elements_end())
    {
        unsigned int node_counter = 0;
        for(auto& node: elem->node_ref_range())
        {
            connectivity[elem_counter][node_counter] = node->id();
            node_counter++;
        }
    }
}

void 
removeDuplicateVerts(libMesh::Mesh& vacuumMesh, libMesh::Mesh& geometryMesh, std::map<int, int>& vacToGeomNodes)
{
    int matches = 0;
    for(auto& geomNode: geometryMesh.local_node_ptr_range())
    {
        for(auto& node: vacuumMesh.local_node_ptr_range())
        {
            if((*node) == (*geomNode))
            {
                matches ++;
                vacToGeomNodes[geomNode->id()] = node->id();
                geometryMesh.delete_node(node);
            }geomTo
        }
    }
}

void 
addGeomVerts(libMesh::Mesh& geometryMesh, libMesh::Mesh& vacuumMesh, std::map<int, int>& vacToGeomNodes)
{
    for(auto& node: geometryMesh.local_node_ptr_range())
    {
        vacuumMesh.add_point(*node);
        vacToGeomNodes[vacuumMesh.max_node_id()] = node->id();
    }
}

void 
createFullGeometry(libMesh::Mesh& geometryMesh,
                   libMesh::Mesh& vacuumMesh)
{
    libMesh::ElemType elem_type, face_type; 
    int num_elem_faces, num_face_nodes;

    std::map<int, int> vacToGeomNodes;
    std::vector<std::vector<unsigned int>> connectivity;
    std::vector<libMesh::dof_id_type> bdr_node_id_list
    std::vector<libMesh::boundary_id_type> bc_id_list

    getElemInfo(elem_type, face_type, 
                elem, num_elem_faces, num_face_nodes);
                
    getGeometryConnectivity(geometryMesh, connectivity);
    getGeometryBoundaries(geometryMesh, bdr_node_id_list, bc_id_list);
    removeDuplicateVerts(vacuumMesh, geometryMesh, vacToGeomNodes);
    addGeomVerts(geometryMesh, vacuumMesh, vacToGeomNodes);

    for(auto& elem_conn: connectivity)
    {
        libMesh::Elem* elem = libMesh::Elem::build(face_type).release();
        for(int j = 0; j < num_face_nodes; j++)
        {
            elem->set_node(j) = surfaceMesh.node_ptr(newNodeIds[connectivity[(i*num_face_nodes)+j]]);
        }
        elem->set_id(i);
        vacuumMesh.add_elem(elem);
    }
}