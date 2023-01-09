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
    changeMeshBasis(sidesetMesh, origin, basisMatrix);
    
    // for(auto& node: sidesetMesh.node_ptr_range())
    // {
    //     // Eigen::Vector to store node coords
    //     Eigen::Vector3d point, newPoint;
    //     // ptr to a boundary side

    //     for(u_int i = 0; i < 3; i++){point(i) = (*node)(i);}

    //     newPoint = calculateLocalCoords(basisMatrix, origin, point);
    //     // newPoint = calculateLocalCoords(basisMatrix, origin, origin);
    //     // std::cout << newPoint.transpose() << std::endl;

    //     double pnt[3];
    //     for(u_int i = 0; i < 3; i++){pnt[i] = newPoint(i);}

    //     libMesh::Point xyz(pnt[0], pnt[1], pnt [2]);
    //     // std::cout << pnt[0] << " " << pnt[1] << " " << pnt[2] << std::endl;
    //     newMesh.add_point(xyz, node->id());
    // }

    // std::cout << "making elems" << std::endl;
    // for(auto& elem: sidesetMesh.element_ptr_range())
    // {
    //     libMesh::Elem* new_elem = libMesh::Elem::build(elem->type()).release();
    //     for(int j = 0; j < elem->n_nodes(); j++)
    //     {
    //         new_elem->set_node(j) = newMesh.node_ptr(elem->node_ref(j).id());
    //     }
    //     newMesh.add_elem(new_elem);
    // }
    // newMesh.set_mesh_dimension(3);
    // newMesh.prepare_for_use();
    auto box = libMesh::MeshTools::create_bounding_box(sidesetMesh);

    Eigen::MatrixXd holes(2, 2);
    Eigen::Vector3d hole1 = {13.648293, 63.404220, 0.000000};
    Eigen::Vector3d hole2 = {34.867462, 70.662737, 0.000000};
    Eigen::Vector3d hole1T = calculateLocalCoords(hole1, origin, basisMatrix);
    Eigen::Vector3d hole2T = calculateLocalCoords(hole2, origin, basisMatrix);
    
    holes << hole1T(0), hole1T(1),
             hole2T(0), hole2T(1);
    std::cout << holes << std::endl;
    libMesh::Mesh remainingBoundary(newMesh.comm());
    remainingBoundary.read("remainingBoundary.e");
    generateCoilFaceBound(sidesetMesh, newMesh, remainingBoundary, holes);
    changeMeshBasis(newMesh, {0, 0, 0}, Eigen::Matrix3d::Identity(), origin, basisMatrix);    
    combineMeshes(1e-05, newMesh, mesh);
}


void changeMeshBasis(libMesh::Mesh& mesh, Eigen::Vector3d newOrigin, Eigen::Matrix3d newBasis, Eigen::Vector3d oldOrigin, Eigen::Matrix3d oldBasis)
{
    libMesh::Mesh meshCopy(mesh);
    mesh.clear();
    for(auto& node: meshCopy.node_ptr_range())
    {
        // Eigen::Vector to store node coords
        Eigen::Vector3d point, newPoint;

        for(u_int i = 0; i < 3; i++){point(i) = (*node)(i);}

        newPoint = calculateLocalCoords(point, newOrigin, newBasis, oldOrigin, oldBasis);
        // newPoint = calculateLocalCoords(basisMatrix, origin, origin);
        // std::cout << newPoint.transpose() << std::endl;

        double pnt[3];
        for(u_int i = 0; i < 3; i++){pnt[i] = newPoint(i);}

        libMesh::Point xyz(pnt[0], pnt[1], pnt [2]);
        // std::cout << pnt[0] << " " << pnt[1] << " " << pnt[2] << std::endl;
        mesh.add_point(xyz, node->id());
    }

    std::cout << "making elems" << std::endl;
    for(auto& elem: meshCopy.element_ptr_range())
    {
        libMesh::Elem* new_elem = libMesh::Elem::build(elem->type()).release();
        for(int j = 0; j < elem->n_nodes(); j++)
        {
            new_elem->set_node(j) = mesh.node_ptr(elem->node_ref(j).id());
        }
        mesh.add_elem(new_elem);
    }
    mesh.prepare_for_use();
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

Eigen::Vector3d calculateLocalCoords(Eigen::Vector3d& point, Eigen::Vector3d newOrigin, Eigen::Matrix3d newBasis, Eigen::Vector3d oldOrigin, Eigen::Matrix3d oldBasis)
{
    Eigen::Vector3d localCoords = newBasis.inverse() * (oldOrigin - newOrigin + (oldBasis * point));
    return localCoords;
}

void generateCoilFaceBound(libMesh::Mesh& mesh, libMesh::Mesh& outputMesh, libMesh::Mesh& remainingBoundary, Eigen::MatrixXd& holes)
{

    Eigen::MatrixXd verts, newVerts;
    Eigen::MatrixXi elems, newElems;
    genSidesetBounds(mesh, remainingBoundary);
    libMeshToIGL(mesh, verts, elems, 2);
    igl::triangle::triangulate(verts, elems, holes, "qY", newVerts, newElems);
    IGLToLibMesh(outputMesh, newVerts, newElems);
    combineMeshes(1e-05, outputMesh, remainingBoundary);
    
}

void genSidesetBounds(libMesh::Mesh& sidesetMesh, double boundLength)
{

    auto box = libMesh::MeshTools::create_bounding_box(sidesetMesh);
    libMesh::Point centre = ((box.max() + box.min())/2);
    std::cout << centre << std::endl;
    libMesh::Point topLeft(centre(0) - (boundLength/2), centre(1) + (boundLength/2));
    libMesh::Point topRight(centre(0) + (boundLength/2), centre(1) + (boundLength/2));
    libMesh::Point bottomLeft(centre(0) - (boundLength/2), centre(1) - (boundLength/2));
    libMesh::Point bottomRight(centre(0) + (boundLength/2), centre(1) - (boundLength/2));

    
    // Vector of points to make it easier to iterate through
    std::vector<libMesh::Point> points{topLeft, topRight, bottomRight, bottomLeft};

    // Starting node ID for points
    libMesh::dof_id_type startingNode = sidesetMesh.max_node_id();
    libMesh::dof_id_type startingElem = sidesetMesh.max_elem_id();
    std::vector<libMesh::dof_id_type> conn{startingNode + 1, startingNode + 2, startingNode + 2, startingNode + 3, startingNode + 3, startingNode + 4, startingNode + 4, startingNode + 1,};
    
    for(u_int i = 0; i<4; i++)
    {
        std::cout << startingNode + (i+1) << std::endl;
        sidesetMesh.add_point(points[i], startingNode + (i+1));
        // conn.push_back(startingNode + (i+1));
    }

    for(u_int i = 0; i<4; i++)
    {
        libMesh::Elem* elem = libMesh::Elem::build(libMesh::ElemType::EDGE2).release();
        for(int j = 0; j < 2; j++)
        {
            std::cout << conn[(i*2)+j] << " ";
            elem->set_node(j) = sidesetMesh.node_ptr(conn[(i*2)+j]);
        }
        std::cout << std::endl;
        elem->set_id(startingElem + (i+1));
        sidesetMesh.add_elem(elem);
    }
    sidesetMesh.prepare_for_use();
}

void genSidesetBounds(libMesh::Mesh& sidesetMesh, libMesh::Mesh& remainingBoundary)
{

    std::unordered_map<u_int, libMesh::dof_id_type> id_map;
    libMesh::Mesh skinnedBound(sidesetMesh.comm());
    getSurface(remainingBoundary, skinnedBound);
    
    // Starting node ID for points
    libMesh::dof_id_type startingNode = sidesetMesh.max_node_id() + 1;
    libMesh::dof_id_type startingElem = sidesetMesh.max_elem_id() + 1;
    
    for(auto& node: skinnedBound.node_ptr_range())
    {
        double pnt[2];
        for(u_int i = 0; i<2; i++){pnt[i] = (*node)(i);}
        libMesh::Point xyz(pnt[0], pnt[1]);
        id_map[node->id()] = startingNode;
        sidesetMesh.add_point(xyz, startingNode);
        startingNode++;
    }

    for(auto& elem: skinnedBound.element_ptr_range())
    {
        libMesh::Elem* newElem = libMesh::Elem::build(elem->type()).release();
        for(int j = 0; j < elem->n_nodes(); j++)
        {
            // std::cout << id_map[elem->node_id(j)] << std::endl;
            newElem->set_node(j) = sidesetMesh.node_ptr(id_map[elem->node_id(j)]);
        }
        newElem->set_id(startingElem++);
        sidesetMesh.add_elem(newElem);
    }

    sidesetMesh.prepare_for_use();
}

void genRemainingBoundary()
{

}

// void genSquare()
// {
    
// }

// void getVoidCoords()
// {
    
// }
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