#include "generateBoundaries.hpp"
#include "removeDuplicateVerts.hpp"

void 
getBoundingEdges(libMesh::Elem* elem, 
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

    unsigned int boundaryElemCount = 0;
    // std::vector<int> boundEdgeList;
    std::vector<libMesh::dof_id_type> element_id_list;
    std::vector<unsigned short int> side_list;
    std::vector<libMesh::boundary_id_type> bc_id_list;
    std::vector<libMesh::dof_id_type> current_node_ids;
    std::vector<int> boundarySides;
    std::vector<std::vector<int>> conn;

    std::map<int, int> newNodeIds;

    // Input data to triangulate our boundary region
    Eigen::MatrixXd V;
    Eigen::MatrixXd H(2,2);
    Eigen::MatrixXi E;
    
    // Matrices to store our boundary
    Eigen::MatrixXd V2;
    Eigen::MatrixXi F2;

    surfaceMesh.get_boundary_info().build_active_side_list(element_id_list, side_list, bc_id_list);
    //Get list of boundary elements
    std::sort(element_id_list.begin(), element_id_list.end());
    std::vector<libMesh::dof_id_type>::iterator newEnd;
    newEnd = std::unique(element_id_list.begin(), element_id_list.end());
    element_id_list.resize(std::distance(element_id_list.begin(), newEnd));

    for(libMesh::dof_id_type& elem_id: element_id_list)
    {
        std::vector<int> boundarySides;
        libMesh::Elem* elem = surfaceMesh.elem_ptr(elem_id);

        getBoundingEdges(elem, element_id_list, boundarySides);

        for(auto& side_num: boundarySides)
        {  
            std::vector<int> side_conn;
            std::unique_ptr<libMesh::Elem> edge = elem->side_ptr(side_num);
            for(auto& node: edge->node_ref_range())
            {
                current_node_ids.push_back(node.id());
                side_conn.push_back(node.id());
            }
            conn.push_back(side_conn);
            boundaryElemCount++;
        }
    }

    std::sort(current_node_ids.begin(), current_node_ids.end());
    newEnd = std::unique(current_node_ids.begin(), current_node_ids.end());
    current_node_ids.resize(std::distance(current_node_ids.begin(), newEnd));

    for(int i = 0; i < (int) current_node_ids.size(); i++)
    {
        newNodeIds[current_node_ids[i]] = i;
    }

    V.resize(4+current_node_ids.size(), 2);
    for(auto& node : current_node_ids)
    {   
        libMesh::Node* node_ptr = surfaceMesh.node_ptr(node);
        V(newNodeIds[node], 0) = (*node_ptr)(0);
        V(newNodeIds[node], 1) = (*node_ptr)(1);
    }

    E.resize(4 + boundaryElemCount, 2);
    for(int elem_num = 0; elem_num < boundaryElemCount; elem_num++)
    {
        for(int node_num = 0; node_num < 2; node_num++)
        {
            E(elem_num, node_num) = newNodeIds[conn[elem_num][node_num]];
        }
    }
    
    libMesh::BoundingBox box = libMesh::MeshTools::create_bounding_box(surfaceMesh);
    std::vector<std::vector<double>> boundingSquare;
    box.max().print();
    boundingSquare.push_back({2*box.max()(0), 2*box.max()(1)});
    boundingSquare.push_back({2*box.max()(0), 2*box.min()(1)});
    boundingSquare.push_back({2*box.min()(0), 2*box.min()(1)});
    boundingSquare.push_back({2*box.min()(0), 2*box.max()(1)});
    


    for(int i = 0; i < 4; i++)
    {
        V(current_node_ids.size() + i, 0) = boundingSquare[i][0];
        V(current_node_ids.size() + i, 1) = boundingSquare[i][1];
    }

    E(boundaryElemCount, 0) = current_node_ids.size();
    E(boundaryElemCount, 1) = current_node_ids.size() + 1;

    E(boundaryElemCount + 1, 0) = current_node_ids.size() + 1;
    E(boundaryElemCount + 1, 1) = current_node_ids.size() + 2;

    E(boundaryElemCount + 2, 0) = current_node_ids.size() + 2;
    E(boundaryElemCount + 2, 1) = current_node_ids.size() + 3;

    E(boundaryElemCount + 3, 0) = current_node_ids.size() + 3;
    E(boundaryElemCount + 3, 1) = current_node_ids.size();


    H <<
    0.035873, 0.017588,
    -16.682617, 14.362826;

    igl::triangle::triangulate(V,E,H,"cdCqa10",V2,F2);

    igl::opengl::glfw::Viewer viewer;
    // viewer.data().set_mesh(V2, F2);
    // viewer.data().add_points(V, Eigen::RowVector3d(1,0,0));
    // viewer.data().add_points(H, Eigen::RowVector3d(200,200,0));
    // viewer.launch();
    Eigen::MatrixXi emptyFaces;
    igl::writeMESH("lol.mesh", V2, emptyFaces,  F2);

    // boundaryMesh.renumber_nodes_and_elements();
    // boundaryMesh.set_mesh_dimension(2); //Should this be 2 or 3???
    // boundaryMesh.set_spatial_dimension(3);
    // boundaryMesh.write("boundaryMeshTester.e");

    // Need H, set of 2D node positions
    // E, E * 2 array of edges 
}




