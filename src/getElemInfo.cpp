#include "getElemInfo.hpp"

void 
getElemInfo(libMesh::ElemType& elem_type, libMesh::ElemType& face_type, 
            libMesh::Elem* element, int& num_elem_sides, int& num_face_nodes)
{
    elem_type = element->type();
    libMesh::Elem* side = element->side_ptr(0).release();
    num_elem_sides = element->n_sides();
    num_face_nodes = side->n_nodes(); 
            
        case (libMesh::HEX27):
            num_elem_sides = 6;
            num_face_nodes = 9;
            face_type = libMesh::QUAD9;
            break;
        
        case (libMesh::TET4):
            num_elem_sides = 4;
            num_face_nodes = 3;
            face_type = libMesh::TRI3;
            break;

        case (libMesh::TET10):
            num_elem_sides = 4;
            num_face_nodes = 6;
            face_type = libMesh::TRI6;
            break;
        // 2D Elems
        case (3):
            num_elem_sides = 3;
            num_face_nodes = 2;
            face_type = libMesh::EDGE2;
            break;
            
        case (libMesh::TRI6):
            num_elem_sides = 6;
            num_face_nodes = 9;
            face_type = libMesh::EDGE3;
            break;
        
        case (libMesh::QUAD4): 
            num_elem_sides = 4;
            num_face_nodes = 2;
            face_type = libMesh::EDGE2;
            break;

        case (libMesh::QUAD9):
            num_elem_sides = 4;
            num_face_nodes = 3;
            face_type = libMesh::EDGE3;
            break;
    }
}
