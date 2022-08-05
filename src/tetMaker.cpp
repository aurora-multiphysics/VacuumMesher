#include<tetMaker.hpp>

void tetrahedraliseVacuumRegion(std::string filename, Eigen::MatrixXd seedPoints)
{   
    std::string path = "../Meshes/";
    // std::string filename = "chimWithBoundRefined.off";
    std::string outputExtension = ".mesh";
    std::string token = filename.substr(0, filename.find("."));
    std::string outputFile = path + "Outputs/" + token + "_out" + outputExtension;
    filename = path + filename;

    Eigen::MatrixXd V;
    Eigen::MatrixXi F;  
    Eigen::MatrixXi T;
   // igl::readMESH("../Meshes/surfaceCube.mesh", V, T, F);
    igl::readOFF(filename, V, F);

    // igl::opengl::glfw::Viewer viewer;
    // viewer.data().set_mesh(V, F);
    // viewer.data().add_points(H, Eigen::RowVector3d(1,0,0));
    // viewer.launch();


    Eigen::MatrixXd R;

    // Eigen::MatrixXd U;
    // Eigen::MatrixXi G;  
    // Eigen::VectorXi J;
    // Eigen::VectorXi I;
    // igl::decimate(V,F,1000000,U,G,J,I);
    // // igl::writeOFF("../Meshes/Outputs/coarse.off", U, G);


    // std::cout << G.size() << std::endl;

    Eigen::MatrixXd TV;
    Eigen::MatrixXi TT;  
    Eigen::MatrixXi TF;
    Eigen::MatrixXd TR;  
    Eigen::MatrixXi TN;
    Eigen::MatrixXi PT;  
    Eigen::MatrixXi FT;
    size_t numRegions;
    igl::copyleft::tetgen::tetrahedralize(V, F, seedPoints, R, "pq2.0CVa0.01", TV, TT, TF, TR, TN, PT, FT, numRegions);
    igl::writeMESH(outputFile, TV, TT, TF);
}
