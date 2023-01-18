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

    // Define new origin as a point on the coil sideset(s), this will be fairly central
    Eigen::Vector3d origin = plane_points.row(0);
    auto start1 = std::chrono::steady_clock::now();
    // Generate basis matrix based on 3 points that define a plane
    getBasisMatrix(basisMatrix, plane_points);

    std::cout << "Basis Matrix retrieved" << std::endl;
    libMesh::Mesh iglBase(newMesh.comm());
    Eigen::MatrixXd V, V_tri;
    Eigen::MatrixXi F, F_tri;
    
    
    libMeshToIGL(sidesetMesh, V, F);
    std::cout << "Changing Mesh Basis" << std::endl;
    
    changeMeshBasis(V, origin, basisMatrix);
    // IGLToLibMesh(iglBase, V, F);

    // Define seed points matrix
    Eigen::MatrixXd seed_points(2, 3);

    // Get the seed points of the coplanar coil boundaries
    getCoplanarSeedPoints(mesh, seed_points);

    // Transform seed points into new coordinate system
    for(u_int i = 0; i<seed_points.rows(); i++)
    {
        Eigen::Vector3d point(seed_points.row(i));
        seed_points.row(i) = calculateLocalCoords(point, origin, basisMatrix);
    }

    Eigen::MatrixXd holes = seed_points.block(0,0,2,2);
    
    // libMesh::Mesh remainingBoundary(newMesh.comm());
    // remainingBoundary.read("remainingBoundary.e");
    
    std::cout << "Generating coil face bound" << std::endl;
    generateCoilFaceBound(V, F, holes, V_tri, F_tri);
    std::cout << "Outputting mesh" << std::endl;
    libMesh::Mesh square(newMesh.comm());
    // IGLToLibMesh(square, V_tri, F_tri);
    const double tol = 1e-05;
    genRemainingBoundary(200, 25, tol, square);
    square.write("BoundaryRemTEst.e");
    
    auto end1 = std::chrono::steady_clock::now();
    std::cout << "Elapsed time in milliseconds: "
    << std::chrono::duration_cast<std::chrono::milliseconds>(end1 - start1).count()
    << " ms" << std::endl;

    // changeMeshBasis(newMesh, {0, 0, 0}, Eigen::Matrix3d::Identity(), origin, basisMatrix);    

    
    // combineMeshes(tol, newMesh, mesh);

    
    // genSquare(3, 4, square);
}


Eigen::Vector3d calculateLocalCoords(Eigen::Vector3d& point, Eigen::Vector3d newOrigin, Eigen::Matrix3d newBasis, Eigen::Vector3d oldOrigin, Eigen::Matrix3d oldBasis)
{
    Eigen::Vector3d localCoords = newBasis.inverse() * (oldOrigin - newOrigin + (oldBasis * point));
    return localCoords;
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

// Change mesh basis for a libIGL (Eigen) mesh
void changeMeshBasis(Eigen::MatrixXd& V, Eigen::Vector3d newOrigin, Eigen::Matrix3d newBasis)
{
    // Inverse the new basis matrix 
    Eigen::Matrix3d newBasisInverse = newBasis.inverse();
    // We assume we are changing basis from bog standard cartesian coords.
    //  i.e. an origin of 0,0,0 and basis vectors of (1,0,0), (0,1,0), (0,0,1)
    Eigen::Matrix3d oldBasis(Eigen::Matrix3d::Identity());
    Eigen::Vector3d oldOrigin(Eigen::Vector3d::Zero());
    // Do row wise operations on
    for(int i = 0; i < V.rows(); i++)
    {
        V.row(i) = newBasisInverse * (oldOrigin - newOrigin + (oldBasis * V.row(i).transpose()));
    }
}

void translateMesh(Eigen::MatrixXd& V, Eigen::Vector3d translationVector)
{
    for(int i = 0; i<V.rows(); i++)
    {
        V.row(i) += translationVector;
    }
}

bool getBasisMatrix(Eigen::Matrix3d& basisMatrix, Eigen::Matrix3d& plane_points)
{   
    // Basis vectors X Y and Z
    Eigen::Vector3d X, Y, Z;
    
    // Vectors between the 3 points that will define our plane
    Eigen::Vector3d AB = (plane_points.row(1) - plane_points.row(0));
    Eigen::Vector3d AC = (plane_points.row(2) - plane_points.row(0));
    
    // Normalise basis vectors so that their magnitude is one, relative to original basis
    X = AB.normalized();
    Z = (AC.cross(AB)).normalized();
    Y = (X.cross(Z)).normalized();
    // Add our basis vectors to 'basisMatrix' matrix
    basisMatrix.col(0) << X;
    basisMatrix.col(1) << Y;
    basisMatrix.col(2) << Z;
    
    return true;
}

void generateCoilFaceBound(Eigen::MatrixXd& V, Eigen::MatrixXi& F, Eigen::MatrixXd& holes, Eigen::MatrixXd& tri_V, Eigen::MatrixXi& tri_F)
{
    Eigen::MatrixXd V_2d = V.block(0,0,V.rows(), 2);
    Eigen::MatrixXi F_2d = F.block(0,0,F.rows(), 2);
    genSidesetBounds(V_2d, F_2d, 200, 25);
    igl::triangle::triangulate(V_2d, F_2d, holes, "qY", tri_V, tri_F);
}

void generateCoilFaceBound(libMesh::Mesh& mesh, libMesh::Mesh& outputMesh, libMesh::Mesh& remainingBoundary, Eigen::MatrixXd& holes)
{

    Eigen::MatrixXd verts, newVerts;
    Eigen::MatrixXi elems, newElems;
    genSidesetBounds(mesh, remainingBoundary);
    libMeshToIGL(mesh, verts, elems, 2);
    igl::triangle::triangulate(verts, elems, holes, "qYa130.0", newVerts, newElems);
    IGLToLibMesh(outputMesh, newVerts, newElems);
    combineMeshes(1e-05, outputMesh, remainingBoundary);    
}

void genSidesetBounds(Eigen::MatrixXd& V, Eigen::MatrixXi& F, double length, int subdivisions)
{
    Eigen::MatrixXd V_square((4*subdivisions), 2);
    Eigen::MatrixXi F_square((4*subdivisions), 2);
    std::cout << "generating square" << std::endl;
    genSquare(V_square, F_square, length, subdivisions);
    std::cout << "Combinging IGL meshes" << std::endl;
    combineIGLMeshes(V, F, V_square, F_square);
    std::cout << "Combinging IGL meshes" << std::endl;

    // auto box = libMesh::MeshTools::create_bounding_box(sidesetMesh);
    // libMesh::Point centre = ((box.max() + box.min())/2);
    // std::cout << centre << std::endl;
    // libMesh::Point topLeft(centre(0) - (boundLength/2), centre(1) + (boundLength/2));
    // libMesh::Point topRight(centre(0) + (boundLength/2), centre(1) + (boundLength/2));
    // libMesh::Point bottomLeft(centre(0) - (boundLength/2), centre(1) - (boundLength/2));
    // libMesh::Point bottomRight(centre(0) + (boundLength/2), centre(1) - (boundLength/2));

    
    // // Vector of points to make it easier to iterate through
    // std::vector<libMesh::Point> points{topLeft, topRight, bottomRight, bottomLeft};

    // // Starting node ID for points
    // libMesh::dof_id_type startingNode = sidesetMesh.max_node_id();
    // libMesh::dof_id_type startingElem = sidesetMesh.max_elem_id();
    // std::vector<libMesh::dof_id_type> conn{startingNode + 1, startingNode + 2, startingNode + 2, startingNode + 3, startingNode + 3, startingNode + 4, startingNode + 4, startingNode + 1,};
    
    // for(u_int i = 0; i<4; i++)
    // {
    //     std::cout << startingNode + (i+1) << std::endl;
    //     sidesetMesh.add_point(points[i], startingNode + (i+1));
    //     // conn.push_back(startingNode + (i+1));
    // }

    // for(u_int i = 0; i<4; i++)
    // {
    //     libMesh::Elem* elem = libMesh::Elem::build(libMesh::ElemType::EDGE2).release();
    //     for(int j = 0; j < 2; j++)
    //     {
    //         std::cout << conn[(i*2)+j] << " ";
    //         elem->set_node(j) = sidesetMesh.node_ptr(conn[(i*2)+j]);
    //     }
    //     std::cout << std::endl;
    //     elem->set_id(startingElem + (i+1));
    //     sidesetMesh.add_elem(elem);
    // }
    // sidesetMesh.prepare_for_use();
    
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

void getCoplanarSeedPoints(libMesh::Mesh& mesh, Eigen::MatrixXd& seed_points, std::string ss1_name, std::string ss2_name)
{   
    // generate separate meshes for two coil sidesets
    std::set<libMesh::boundary_id_type> ss1_id, ss2_id;
    ss1_id.insert(mesh.boundary_info->get_id_by_name(ss1_name));
    ss2_id.insert(mesh.boundary_info->get_id_by_name(ss2_name)); 
    libMesh::Mesh ss1(mesh.comm());
    libMesh::Mesh ss2(mesh.comm());
    mesh.boundary_info->sync(ss1_id, ss1);
    mesh.boundary_info->sync(ss2_id, ss2);

    libMesh::Point centre1, centre2;

    // Create a bounding box around these 2D sidesets to figure out where a seeding point should be placed
    auto box1 = libMesh::MeshTools::create_bounding_box(ss1);
    auto box2 = libMesh::MeshTools::create_bounding_box(ss2);

    centre1 = (box1.max() + box1.min())/2;
    std::cout << centre1 << std::endl;
    centre2 = (box2.max() + box2.min())/2;
    std::cout << centre2 << std::endl;
    //
    for(u_int i = 0; i<3; i++)
    {
        seed_points.row(0)(i) = centre1(i);
        seed_points.row(1)(i) = centre2(i);
    } 

}

void genSquare(Eigen::MatrixXd& V, Eigen::MatrixXi& F, double length, int subdivisions)
{
    // Eigen::MatrixXd V((4*subdivisions), 2);
    // Eigen::MatrixXi F((4*subdivisions), 2);
    double increment  = (double)(length/subdivisions);
    double offset = length/2;

    long int row_num = 0;
    for(int i = 0; i <= subdivisions; i++)
    {
        V.row(row_num) << -offset + (i*increment), -offset;
        row_num++;
    }

    for(int i = 1; i <= subdivisions; i++)
    {
        V.row(row_num) << offset, (i*increment) - offset;
        row_num++;
    }

    for(int i = 1; i <= subdivisions; i++)
    {
        V.row(row_num) << offset - (i*increment), offset;
        row_num++;
    }

    for(int i = 1; i <= subdivisions; i++)
    {
        V.row(row_num) << -offset, offset - (i*increment);
        row_num++;
    }

    for(int i = 0; i < (4*subdivisions)-1; i++)
    {
        F(i, 0) = i;
        F(i, 1) = i+1;
    }
    F(((4*subdivisions)-1), 0) = 0;
    F(((4*subdivisions)-1), 1) = (4*subdivisions)-1;
    // Eigen::MatrixXd holes;
    // Eigen::MatrixXd squareVerts;
    // Eigen::MatrixXi squareElems;
    // igl::triangle::triangulate(V, F, holes, "qY", squareVerts, squareElems);

    // Eigen::Matrix3d x_rot_base;
    // x_rot_base << 1, 0, 0,
    //               0, 0, -1,
    //               0, 1, 0;

    // Eigen::Matrix3d y_rot_base;
    // y_rot_base << 0, 0, -1,
    //               0, 1, 0,
    //               1, 0, 0;

    // IGLToLibMesh(square, squareVerts, squareElems);
    // square.write("squaretest1.e");
    // squareVerts.conservativeResize(squareVerts.rows(), squareVerts.cols()+1);
    // squareVerts.col(squareVerts.cols()-1) = Eigen::VectorXd::Zero(squareVerts.rows());
    
    // squareVerts = squareVerts * x_rot_base;
    // std::cout << squareVerts << std::endl;
    // IGLToLibMesh(square, squareVerts, squareElems);
    // square.write("squaretest3.e");
    // PROBLEM, INITAL MATRIX IS 2D, WE NEED TO ADD A Z COLUMN

}

void combineIGLMeshes(Eigen::MatrixXd& V1, Eigen::MatrixXi& F1, Eigen::MatrixXd& V2, Eigen::MatrixXi& F2)
{
    if(V1.cols() != V2.cols())
    {
        throw std::invalid_argument("combineIGLMeshes can only combined meshes of the same dimension");
        abort;
    }
    // Store rows in V1 before we resize it
    int initial_node_rows = V1.rows();
    int initial_elem_rows = F1.rows();
    // Resize V1 to store info from V2
    V1.conservativeResize(V1.rows() + V2.rows(), V1.cols());
    F1.conservativeResize(F1.rows() + F2.rows(), F1.cols());
    
    for(int i = 0; i < V2.rows(); i++)
    {
        V1.row(initial_node_rows + i) = V2.row(i);
    }

    for(int i = 0; i < F2.rows(); i++)
    {
        F1.row(initial_elem_rows + i) = F2.row(i) + Eigen::VectorXi::Constant(F1.cols(), initial_node_rows);
    }
}

void genRemainingBoundary(double length, int subdivisions, double tol, libMesh::Mesh& test)
{
    Eigen::MatrixXd V(4*subdivisions, 2); 
    Eigen::MatrixXi F(4*subdivisions, 2);

    Eigen::MatrixXd V_tri(4*subdivisions, 2); 
    Eigen::MatrixXi F_tri(4*subdivisions, 3);

    Eigen::MatrixXd seeds;

    genSquare(V, F, length, subdivisions);

    igl::triangle::triangulate(V, F, seeds, "qY", V_tri, F_tri);


    Eigen::Matrix3d x_rot_base;
    x_rot_base << 1, 0, 0,
                  0, 0, -1,
                  0, 1, 0;

    Eigen::Matrix3d y_rot_base;
    y_rot_base << 0, 0, -1,
                  0, 1, 0,
                  1, 0, 0;

    // Resice V_tri to work in 3d, as it is currently a 2D mesh
    //  this is done just by adding a column of zeroes
  

    V_tri.conservativeResize(V_tri.rows(), V_tri.cols()+1);
    V_tri.col(V_tri.cols()-1) = Eigen::VectorXd::Zero(V_tri.rows());

    F_tri.conservativeResize(F_tri.rows(), F_tri.cols()+1);
    F_tri.col(F_tri.cols()-1) = Eigen::VectorXi::Zero(F_tri.rows());
    Eigen::MatrixXi F_tri_2 = F_tri;


    Eigen::MatrixXd V_tri_2 = V_tri;
    V_tri_2 *= x_rot_base; 

    combineMeshes(tol, V_tri, F_tri, V_tri_2, F_tri_2);

    IGLToLibMesh(test, V_tri, F_tri);
    // NOT TO SELF, CURRENTLY NOT WORKING



    


}
