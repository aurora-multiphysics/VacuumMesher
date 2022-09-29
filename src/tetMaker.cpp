#include<tetMaker.hpp>

void tetrahedraliseVacuumRegion(std::string filename, std::string outname, Eigen::MatrixXd seedPoints)
{   
    Eigen::MatrixXd V;
    Eigen::MatrixXi F;  
    Eigen::MatrixXi T;
    igl::readOFF(filename, V, F);
    // igl::opengl::glfw::Viewer viewer;
    // viewer.data().set_mesh(V, F);
    // viewer.data().add_points(H, Eigen::RowVector3d(1,0,0));
    // viewer.launch();

    Eigen::MatrixXd R;
    Eigen::MatrixXd TV;
    Eigen::MatrixXi TT;  
    Eigen::MatrixXi TF;
    Eigen::MatrixXd TR;  
    Eigen::MatrixXi TN;
    Eigen::MatrixXi PT;  
    Eigen::MatrixXi FT;
    size_t numRegions;
    
    igl::copyleft::tetgen::tetrahedralize(V, F, seedPoints, R, "pqCVY", TV, TT, TF, TR, TN, PT, FT, numRegions);
    
    //Empty matrix. If we put TF as an argument instead, writeMESH will also output the original skinned
    // mesh as a block. This results in hissy fits if you try and skin the mesh again, as the tri facets
    // from the extra block overlap with the faces of the tets and everything goes haywire
    Eigen::MatrixXi emptyFaces;
    igl::writeMESH(outname, TV, TT, TF);
}


Eigen::MatrixXd getSeeds(std::string filename)
{
    std::string path = "./Meshes/";
    std::string offFilename = filename.substr(0, filename.find(".")) + ".off";
    std::string pyCommStr = "python3 ./pythonScripts/convertMesh.py " + filename;
    const char* pyComm = pyCommStr.c_str();
    system(pyComm);

    std::cout << offFilename << std::endl;
    Eigen::MatrixXd V;
    Eigen::MatrixXi F;  
    Eigen::MatrixXd N_faces;

    igl::readOFF(path+offFilename, V, F);
    Eigen::MatrixXd seed_points(F.rows(), 3);
    igl::per_face_normals(V,F,N_faces);

    auto start = std::chrono::high_resolution_clock::now(); 
    for(int i; i < N_faces.rows(); i++)
    {
        std::vector<double> seed(3, 0);
        std::vector<double> normal = {N_faces.row(i)[0], N_faces.row(i)[1], N_faces.row(i)[2]};
        std::vector<int> verts = {F.row(i)[0], F.row(i)[1], F.row(i)[2]};
        for(auto& vert: verts){
            seed[0]+= (V.row(vert)[0] - 1e-10*normal[0])/3; // x component of seed
            seed[1]+= (V.row(vert)[1]- 1e-10*normal[1])/3; // y component of seed
            seed[2]+= (V.row(vert)[2]- 1e-10*normal[2])/3; // z component of seed
        }
        seed_points.row(i) << seed[0], seed[1], seed[2];
        // seed_points.row(i) << 1,3,2;
    }

    auto end = std::chrono::high_resolution_clock::now(); 
    auto time = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
    std::cout << time << std::endl;
    std::cout << seed_points.rows() << " seed points." << std::endl;
    // std::cout << seed_points << std::endl;
    return seed_points;
}