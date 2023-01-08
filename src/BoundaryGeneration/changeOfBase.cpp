#include "BoundaryGeneration/changeOfBase.hpp"

void getBasisChangeMesh(libMesh::Mesh& mesh, libMesh::Mesh& sidesetMesh,libMesh::Mesh& newMesh)
{
    // Eigen stuff
    Eigen::Matrix3d plane_points;

    // Ptr to out sideset element
    std::unique_ptr<libMesh::Elem> bd_side_ptr;

    std::vector<libMesh::dof_id_type> element_id_list; 
    std::vector<unsigned short int> side_list;
    std::vector<libMesh::boundary_id_type>bc_id_list;
    // Populate boundary info
    mesh.get_boundary_info().build_side_list(element_id_list, side_list, bc_id_list);

    libMesh::boundary_id_type coil_in_id = mesh.get_boundary_info().get_id_by_name("coil_in");

    for(u_int i = 0; i < bc_id_list.size(); i++)
    {
        if(bc_id_list[i] == coil_in_id)
        {
            bd_side_ptr = (mesh.elem_ptr(element_id_list[i])->side_ptr(side_list[i]));
            break;
        }
    }

    for(u_int node_id = 0; node_id < bd_side_ptr->n_nodes(); node_id++)
    {
        libMesh::Node node = bd_side_ptr->node_ref(node_id);
        plane_points.row(node_id) << node(0), node(1), node(2);
    }    
    Eigen::Matrix3d basisMatrix;
    Eigen::Vector3d origin = plane_points.row(0);
    getBasisMatrix(basisMatrix, plane_points);

    for(auto& node: sidesetMesh.node_ptr_range())
    {

        // Eigen::Vector to store node coords
        Eigen::Vector3d point, newPoint;
        // ptr to a boundary side

        for(u_int i = 0; i < 3; i++){point(i) = (*node)(i);}

        newPoint = calculateLocalCoords(basisMatrix, origin, point);
        // newPoint = calculateLocalCoords(basisMatrix, origin, origin);
        // std::cout << newPoint.transpose() << std::endl;

        double pnt[3];
        for(u_int i = 0; i < 3; i++){pnt[i] = newPoint(i);}

        libMesh::Point xyz(pnt[0], pnt[1], pnt [2]);
        // std::cout << pnt[0] << " " << pnt[1] << " " << pnt[2] << std::endl;
        newMesh.add_point(xyz, node->id());
    }

    std::cout << "making elems" << std::endl;
    for(auto& elem: sidesetMesh.element_ptr_range())
    {
        libMesh::Elem* new_elem = libMesh::Elem::build(elem->type()).release();
        for(int j = 0; j < elem->n_nodes(); j++)
        {
            new_elem->set_node(j) = newMesh.node_ptr(elem->node_ref(j).id());
        }
        newMesh.add_elem(new_elem);
    }
    // newMesh.set_mesh_dimension(3);
    newMesh.prepare_for_use();
    
    auto box = libMesh::MeshTools::create_bounding_box(newMesh);
    Eigen::MatrixXd holes;
    // for(u_int i = 0; i<2; i++){holes(0, i) = ((box.max()(i) + box.min()(i))/2);}
    std::cout << holes << std::endl;
    libMesh::Mesh triangulation(newMesh.comm());
    generateTriangleBound(newMesh, triangulation, holes);

}

bool getBasisMatrix(Eigen::Matrix3d& basisMatrix, Eigen::Matrix3d& plane_points)
{   
    Eigen::Vector3d X, Y, Z;
    
    Eigen::Vector3d AB = (plane_points.row(1) - plane_points.row(0));
    Eigen::Vector3d AC = (plane_points.row(2) - plane_points.row(0));
    
    // These are correct
    X = AB.normalized();
    Z = (AC.cross(AB)).normalized();
    Y = (X.cross(Z)).normalized();
    basisMatrix.col(0) << X;
    basisMatrix.col(1) << Y;
    basisMatrix.col(2) << Z;
    
    return true;
}

Eigen::Vector3d calculateLocalCoords(Eigen::Matrix3d& basisMatrix, Eigen::Vector3d& origin, Eigen::Vector3d& point)
{
    Eigen::Matrix3d identity = Eigen::Matrix3d::Identity();
    Eigen::Vector3d localCoords = basisMatrix.inverse() * (-origin + (identity * point));
    return localCoords;
}

void generateTriangleBound(libMesh::Mesh& mesh, libMesh::Mesh& outputMesh, Eigen::MatrixXd& holes)
{
    Eigen::MatrixXd verts, newVerts;
    Eigen::MatrixXi elems, newElems;
    libMeshToIGL(mesh, verts, elems, 2);
    // igl::opengl::glfw::Viewer viewer;
    // viewer.data().set_mesh(verts, elems);
    // viewer.launch();
    // std::cout << "converted to igl" << std::endl;
    igl::triangle::triangulate(verts, elems, holes, "a0.005q", newVerts, newElems);
    IGLToLibMesh(outputMesh, newVerts, newElems);
    // std::cout << "converted to lib" << std::endl;
    outputMesh.write("triangles.e");

}

void getVoidCoords()
{
    
}
// void doubleCheck(Eigen::Matrix3d& basisMatrix, Eigen::Vector3d& origin, Eigen::Vector3d& point, Eigen::Vector3d& initialPoint)
// {
//     std::cout << origin + (basisMatrix * point) << std::endl;
//     if((origin + (basisMatrix * point)) != initialPoint)
//     {
//         std::cout << "NOT WORKING" << std::endl;
//     }
//     else{
//         std::cout << "working" << std::endl;
//     }
// }