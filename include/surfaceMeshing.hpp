#include "libmesh/elem.h"
#include "libmesh/mesh.h"
#include "libmesh/libmesh.h"
#include "libmesh/enum_io_package.h"
#include "libmesh/enum_elem_type.h"


// using namespace libMesh;

void getSurface(libMesh::Mesh& mesh, libMesh::Mesh& surfaceMesh, std::set<int>& elSet);

int getNumSurfaceElems();

void isElementSurface(libMesh::Elem* element, std::set<int>& elSet, std::vector<int>& surfaceFaces);

// libMesh::Mesh surfaceMesh(init.comm());

void getElemInfo(libMesh::ElemType& elem_type, libMesh::ElemType& face_type, 
                 libMesh::Elem* element, int& num_elem_faces, int& num_face_nodes);