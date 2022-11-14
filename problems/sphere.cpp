#include<iostream>
#include"tetMaker.hpp"
#include"visualiseSeedPoints.hpp"

int main(int argc, char** argv)
{   
    std::string path = "../Meshes/SurfaceMeshes/";
    std::string filename = "spherewBound.off";
    std::string outputExtension = ".mesh";
    std::string token = filename.substr(0, filename.find("."));
    std::string outputFile = path + token + "_out" + outputExtension;
    filename = path + filename;
    std::cout << outputFile << std::endl;

    Eigen::MatrixXd H(2, 3);
    H << 
     -0, 0, 0,
     -0, 0.3, 0;
 
     
    visualiseSeedPoints(filename, H);
    // tetrahedraliseVacuumRegion(filename, outputFile, H);
}
