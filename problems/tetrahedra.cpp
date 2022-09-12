#include<iostream>
#include <igl/read_triangle_mesh.h>
#include <igl/randperm.h>
#include <igl/orientable_patches.h>
#include <igl/slice.h>
#include <igl/hsv_to_rgb.h>
#include <igl/embree/reorient_facets_raycast.h>
#include <igl/opengl/glfw/Viewer.h>
#include <chrono>
#include <igl/copyleft/tetgen/tetrahedralize.h>
#include <igl/writeMESH.h>
#include <igl/writeOFF.h>
#include <igl/decimate.h>
#include <igl/opengl/glfw/Viewer.h>

int main()
{   
    std::string path = "../Meshes/";
    std::string filename = "chimWithBoundRefined.off";
    std::string outputExtension = ".mesh";
    std::string token = filename.substr(0, filename.find("."));
    std::string outputFile = path + "Outputs/" + token + "_out" + outputExtension;
    filename = path + filename;
    std::cout << outputFile << std::endl;

    Eigen::MatrixXd V;
    Eigen::MatrixXi F;  
    Eigen::MatrixXi T;
   // igl::readMESH("../Meshes/surfaceCube.mesh", V, T, F);
    igl::readOFF(filename, V, F);
    // std::cout << F << std::endl;

    // Eigen::MatrixXd H(1, 3);
    // H << 
    // 0.0 ,0.0, 0.0;

    Eigen::MatrixXd H(2, 3);
    H << 
     -0.687560, -0.395, 1.192500,
     0.418,0.05,0.1;
     
    //  0.124686, 0.0, -0.212734;
     



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
    igl::copyleft::tetgen::tetrahedralize(V, F, H, R, "pq2.0CV", TV, TT, TF, TR, TN, PT, FT, numRegions);
    igl::writeMESH(outputFile, TV, TT, TF);
}
