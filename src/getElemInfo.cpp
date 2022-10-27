#include "getElemInfo.hpp"

void 
getElemInfo(libMesh::ElemType& elem_type, libMesh::ElemType& face_type, 
            libMesh::Elem* element, int& num_elem_sides, int& num_face_nodes)
{
    std::cout << "Getting element type" << std::endl;
    elem_type = element->type();
    switch (elem_type)
    {
        // 3D Elems
        case (libMesh::HEX8):
            num_elem_sides = 6;
            num_face_nodes = 4;
            face_type = libMesh::QUAD4;
            break;
            
        case (libMesh::HEX27):
            num_elem_sides = 6;
            num_face_nodes = 9;
            face_type = libMesh::QUAD9;
            break;
        
        case (libMesh::TET4):
            std::cout << "TET4 elems detected\n"; 
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
            std::cout << "TRI3 elems detected" << std::endl;
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
