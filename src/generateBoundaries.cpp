#include "generateBoundaries.hpp"

void 
isBoundingEdge(libMesh::Elem* elem, 
                 std::vector<libMesh::dof_id_type>& element_id_list,
                 std::vector<int>& boundarySides)
{
    for(int side = 0; side < elem->n_sides(); side++)
    {
        auto neighbor = elem->neighbor_ptr(side);
        if(!std::binary_search(element_id_list.begin(), element_id_list.end(), neighbor->id()))
        {
            boundarySides.push_back(side);
            // std::cout << "found" << std::endl;
        }
    }     
}

void 
createEdgeMesh(libMesh::Mesh& surfaceMesh, libMesh::Mesh& boundaryMesh)
{
    // libMesh element properties
    libMesh::ElemType elem_type, face_type; 
    int num_elem_faces, num_face_nodes;

    unsigned int boundaryElemCount = 0;

    std::vector<libMesh::dof_id_type> element_id_list;
    std::vector<libMesh::dof_id_type> current_node_ids;
    std::vector<libMesh::boundary_id_type> bc_id_list;
    std::vector<std::vector<int>> connec;
    std::vector<unsigned short int> side_list;
    std::vector<int> boundarySides;
    // Map from current node ids to the node ids that will be used in the boundary mesh
    std::map<int, int> newNodeIds;

    libMesh::Elem* elem_ptr = surfaceMesh.elem_ptr(0);
    //Return via arg element properties
    getElemInfo(elem_type, face_type, 
            elem_ptr, num_elem_faces, num_face_nodes);

    // Get sidesets of mesh
    surfaceMesh.get_boundary_info().build_active_side_list(element_id_list, side_list, bc_id_list);

    // Get list of bounding elements. Despite having element_id_list and side_list, we can't use these
    // to know explicitly which edges are the bounding edges. This is because the sidelist will contain
    //  the sides of all the elements, not just the sides at the very edges of the sideset
    std::sort(element_id_list.begin(), element_id_list.end());
    std::vector<libMesh::dof_id_type>::iterator newEnd;
    newEnd = std::unique(element_id_list.begin(), element_id_list.end());
    element_id_list.resize(std::distance(element_id_list.begin(), newEnd));


    // Get connectivity data/ node ids for the bounding edge mesh
    for(libMesh::dof_id_type& elem_id: element_id_list)
    {
        std::vector<int> boundarySides;
        libMesh::Elem* elem = surfaceMesh.elem_ptr(elem_id);

        isBoundingEdge(elem, element_id_list, boundarySides);

        for(auto& side_num: boundarySides)
        {  
            std::vector<int> side_conn;
            std::unique_ptr<libMesh::Elem> edge = elem->side_ptr(side_num);
            for(auto& node: edge->node_ref_range())
            {
                current_node_ids.push_back(node.id());
                side_conn.push_back(node.id());
            }
            connec.push_back(side_conn);
            boundaryElemCount++;
        }
    }

    // Sort current node IDs and remove duplicates
    std::sort(current_node_ids.begin(), current_node_ids.end());
    newEnd = std::unique(current_node_ids.begin(), current_node_ids.end());
    current_node_ids.resize(std::distance(current_node_ids.begin(), newEnd));
    for(int i = 0; i < (int) current_node_ids.size(); i++)
    {
        newNodeIds[current_node_ids[i]] = i;
    }

    //Add nodes to new mesh, with their new node numberings
    for(auto& node : current_node_ids)
    {   
        libMesh::Node* node_ptr = surfaceMesh.node_ptr(node);
        double pnt[3];
        pnt[0] = (*node_ptr)(0);
        pnt[1] = (*node_ptr)(1);
        pnt[2] = (*node_ptr)(2);
        libMesh::Point xyz(pnt[0], pnt[1], pnt[2]);
        boundaryMesh.add_point(xyz, newNodeIds[node]);
    }

    //Add new elems to boundaryMesh
    for(int i = 0; i < boundaryElemCount; i++)
    {
        libMesh::Elem* elem = libMesh::Elem::build(face_type).release();
        
        for(int j = 0; j < num_face_nodes; j++)
        {
            elem->set_node(j) = boundaryMesh.node_ptr(newNodeIds[connec[i][j]]);
        }

        elem->set_id(i);
        boundaryMesh.add_elem(elem);
    }
    boundaryMesh.set_mesh_dimension(1);
    boundaryMesh.set_spatial_dimension(2);
    boundaryMesh.prepare_for_use();

    createBoundingPlane(surfaceMesh, boundaryMesh, 300);
    boundaryMesh.write("boundaryOut.e");
}

void
createBoundingPlane(libMesh::Mesh& surfaceMesh, libMesh::Mesh& boundaryMesh, double boxSize)
{
    libMesh::BoundingBox box = libMesh::MeshTools::create_bounding_box(surfaceMesh);
    double pnt[3];
    pnt[0] = boxSize/2;
    pnt[1] = boxSize/2;
    std::cout << boundaryMesh.max_node_id() << std::endl;
    libMesh::Point xy(pnt[0], pnt[1]);
    boundaryMesh.add_point(xy, boundaryMesh.max_node_id());

    pnt[0] = -boxSize/2;
    pnt[1] = boxSize/2;
    libMesh::Point xy1(pnt[0], pnt[1]);
    boundaryMesh.add_point(xy1, boundaryMesh.max_node_id());

    pnt[0] = boxSize/2;
    pnt[1] = -boxSize/2;
    libMesh::Point xy2(pnt[0], pnt[1]);
    boundaryMesh.add_point(xy2, boundaryMesh.max_node_id());

    pnt[0] = -boxSize/2;
    pnt[1] = -boxSize/2;
    libMesh::Point xy3(pnt[0], pnt[1]);
    boundaryMesh.add_point(xy3, boundaryMesh.max_node_id());

    std::cout << boundaryMesh.max_node_id() << std::endl;


    for(int i = 3; i > 0; i--)
    {
        libMesh::Elem* elem = libMesh::Elem::build(libMesh::EDGE2).release();
        
        for(int j = 0; j < 2; j++)
        {
            elem->set_node(j) = boundaryMesh.node_ptr(boundaryMesh.max_node_id() - i + j);
        }

        boundaryMesh.add_elem(elem);
    }
    std::cout << "Check" << std::endl;


    libMesh::Elem* elem = libMesh::Elem::build(libMesh::EDGE2).release();
    elem->set_node(0) = boundaryMesh.node_ptr(boundaryMesh.max_node_id());
    elem->set_node(1) = boundaryMesh.node_ptr(boundaryMesh.max_node_id()-3);
    boundaryMesh.add_elem(elem);
    std::cout << "Check" << std::endl;

    boundaryMesh.set_mesh_dimension(1);
    boundaryMesh.set_spatial_dimension(2);
    boundaryMesh.prepare_for_use();
}

// void
// createTriangulation(libMesh::Mesh& surfaceMesh, libMesh::Mesh& boundaryMesh)
// {

//     unsigned int boundaryElemCount = 0;
//     // std::vector<int> boundEdgeList;

//     std::map<int, int> newNodeIds;

//     // Input data to triangulate our boundary region
//     Eigen::MatrixXd V;
//     Eigen::MatrixXd H(2,2);
//     Eigen::MatrixXi E;
    
//     // Matrices to store our boundary
//     Eigen::MatrixXd V2;
//     Eigen::MatrixXi F2;

//     surfaceMesh.get_boundary_info().build_active_side_list(element_id_list, side_list, bc_id_list);
//     //Get list of boundary elements
//     std::sort(element_id_list.begin(), element_id_list.end());
//     std::vector<libMesh::dof_id_type>::iterator newEnd;
//     newEnd = std::unique(element_id_list.begin(), element_id_list.end());
//     element_id_list.resize(std::distance(element_id_list.begin(), newEnd));

  

//     V.resize(4+current_node_ids.size(), 2);
//     for(auto& node : current_node_ids)
//     {   
//         libMesh::Node* node_ptr = surfaceMesh.node_ptr(node);
//         V(newNodeIds[node], 0) = (*node_ptr)(0);
//         V(newNodeIds[node], 1) = (*node_ptr)(1);
//     }

//     E.resize(4 + boundaryElemCount, 2);
//     for(int elem_num = 0; elem_num < boundaryElemCount; elem_num++)
//     {
//         for(int node_num = 0; node_num < 2; node_num++)
//         {
//             E(elem_num, node_num) = newNodeIds[conn[elem_num][node_num]];
//         }
//     }
    
//     libMesh::BoundingBox box = libMesh::MeshTools::create_bounding_box(surfaceMesh);
//     std::vector<std::vector<double>> boundingSquare;
//     box.max().print();
//     boundingSquare.push_back({2*box.max()(0), 2*box.max()(1)});
//     boundingSquare.push_back({2*box.max()(0), 2*box.min()(1)});
//     boundingSquare.push_back({2*box.min()(0), 2*box.min()(1)});
//     boundingSquare.push_back({2*box.min()(0), 2*box.max()(1)});
    
//     H <<
//     0.035873, 0.017588,
//     -16.982617, 14.362826;

//     // igl::triangle::triangulate(V,E,H,"cdCqa10",V2,F2);

//     igl::opengl::glfw::Viewer viewer;
//     viewer.data().set_mesh(V, E);
//     // viewer.data().add_points(V, Eigen::RowVector3d(1,0,0));
//     // viewer.data().add_points(H, Eigen::RowVector3d(200,200,0));
//     viewer.launch();
//     Eigen::MatrixXi emptyFaces;
//     igl::writeMESH("lol.mesh", V2, emptyFaces,  F2);

//     // boundaryMesh.renumber_nodes_and_elements();
//     // boundaryMesh.set_mesh_dimension(2); //Should this be 2 or 3???
//     // boundaryMesh.set_spatial_dimension(3);
//     // boundaryMesh.write("boundaryMeshTester.e");

//     // Need H, set of 2D node positions
//     // E, E * 2 array of edges 
// }




