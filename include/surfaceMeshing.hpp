#include "libmesh/elem.h"
#include "libmesh/mesh.h"
#include "libmesh/libmesh.h"
#include "libmesh/enum_io_package.h"
#include "libmesh/enum_elem_type.h"


// using namespace libMesh;

void getSurface(libMesh::Mesh& mesh, libMesh::Mesh& surfaceMesh, std::vector<int>& elSet);

void getSurface(libMesh::Mesh& mesh, libMesh::Mesh& surfaceMesh);

int getNumSurfaceElems();

void isElementSurface(libMesh::Elem* element, std::vector<int>& elSet, std::vector<int>& surfaceFaces);

void isElementSurface(libMesh::Elem* element, std::vector<int>& surfaceFaces);

void getElemInfo(libMesh::ElemType& elem_type, libMesh::ElemType& face_type, 
                 libMesh::Elem* element, int& num_elem_faces, int& num_face_nodes);

void groupElems(libMesh::Mesh mesh, std::vector<std::vector<libMesh::dof_id_type>>& groups);

void saveGroupedElems(libMesh::LibMeshInit& init, libMesh::Mesh& surfaceMesh, 
                      std::vector<std::vector<libMesh::dof_id_type>>& groups,
                      std::string componentFilename);