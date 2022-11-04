#include"meshBoolean.hpp"


void genBooleanBound(libMesh::Mesh& boundaryMesh, 
                     libMesh::Mesh& surfaceMesh, 
                     libMesh::Mesh& mesh)
{   
    // Normals for visualising
    // Eigen::MatrixXd N_faces;

    // Eigen::MatrixXd VA,VB,VC;
    // Eigen::MatrixXi FA, FB, FC;  
    // Eigen::VectorXi J,surfaceMeshI;
    // libMeshToIGL(surfaceMesh, VA, FA);
    // libMeshToIGL(boundaryMesh, VB, FB);

    // std::vector<libMesh::dof_id_type> element_id;
    // std::vector<short unsigned int> side_id;
    // std::vector<libMesh::boundary_id_type> bd_id;
    // surfaceMesh.get_boundary_info().build_side_list(element_id, side_id, bd_id);
    // for(int i = 0; i<FA.rows(); i++)
    // {   
    //     if(!std::binary_search(element_id.begin(), element_id.end(), i))
    //     {
    //         reverseNormal(i, FA);
    //     }
    // }

    // igl::MeshBooleanType boolean_type(igl::MESH_BOOLEAN_TYPE_UNION);

    // igl::copyleft::cgal::mesh_boolean(VA,FA,VB,FB,boolean_type,VC,FC,J);
    // IGLToLibMesh(mesh, VC, FC);    
    CGAL::Polyhedron_3<CGAL::Exact_predicates_exact_constructions_kernel, CGAL::Polyhedron_items_with_id_3> pol;
    bool ol = libmeshToCGAL(surfaceMesh, pol);
}
