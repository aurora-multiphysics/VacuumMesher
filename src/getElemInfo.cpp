#include "getElemInfo.hpp"

void 
getElemInfo(libMesh::ElemType& elem_type, libMesh::ElemType& face_type, 
            libMesh::Elem* element, int& num_elem_sides, int& num_face_nodes)
{
    elem_type = element->type();
    std::unique_ptr<libMesh::Elem> side = element->side_ptr(0);
    num_elem_sides = element->n_sides();
    num_face_nodes = side->n_nodes();            
}
