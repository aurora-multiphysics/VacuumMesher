#pragma once

#include "libmesh/elem.h"
#include "libmesh/mesh.h"
#include "libmesh/libmesh.h"
#include "libmesh/mesh_tools.h"
#include "libmesh/boundary_info.h"
#include "libmesh/enum_io_package.h"
#include "libmesh/enum_elem_type.h"


void 
getElemInfo(libMesh::ElemType& elem_type, libMesh::ElemType& face_type, 
            libMesh::Elem* element, int& num_elem_faces, int& num_face_nodes);