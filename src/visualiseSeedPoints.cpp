#include"visualiseSeedPoints.hpp"

void visualiseSeedPoints(std::string filename, Eigen::MatrixXd seedPoints)
{   
    Eigen::MatrixXd V;
    Eigen::MatrixXi F;  
    Eigen::MatrixXi T;
    // igl::readOFF(filename, V, F);
    igl::opengl::glfw::Viewer viewer;
    // viewer.data().set_mesh(V, F);
    viewer.data().add_points(seedPoints, Eigen::RowVector3d(1,0,0));
    viewer.launch();
}