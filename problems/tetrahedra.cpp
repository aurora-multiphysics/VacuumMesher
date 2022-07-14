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

int main()
{
    Eigen::MatrixXd V;
    Eigen::MatrixXi F;  
    Eigen::MatrixXi T;
    // igl::readMESH("../Meshes/surfaceCube.mesh", V, T, F);
    igl::readOFF("../Meshes/CubeinACube.off", V, F);
    std::cout << F << std::endl;

    // Eigen::MatrixXd H;
    Eigen::MatrixXd H(1, 3);
    H << 
    0.0 ,0.0, 0.0;


    Eigen::MatrixXd R;



    Eigen::MatrixXd TV;
    Eigen::MatrixXi TT;  
    Eigen::MatrixXi TF;
    Eigen::MatrixXd TR;  
    Eigen::MatrixXi TN;
    Eigen::MatrixXi PT;  
    Eigen::MatrixXi FT;
    size_t numRegions;

    igl::copyleft::tetgen::tetrahedralize(V, F, H, R, "pqa0.01", TV, TT, TF, TR, TN, PT, FT, numRegions);
    igl::writeMESH("../Meshes/Outputs/internalCubeOfftest.mesh", TV, TT, TF);
}
