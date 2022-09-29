#include<iostream>
#include"tetMaker.hpp"
#include"visualiseSeedPoints.hpp"

int main(int argc, char** argv)
{   
    std::string path = "./Meshes/";
    std::string filename = "chimNoLegsBound.off";
    std::string outputExtension = ".mesh";
    std::string token = filename.substr(0, filename.find("."));
    std::string outputFile = path + token + "_out" + outputExtension;
    filename = path + filename;
    // std::cout << outputFile << std::endl;


    Eigen::MatrixXd V;
    Eigen::MatrixXi F;  
    igl::readOFF(filename, V, F);
    Eigen::MatrixXd seed_points = getSeeds("chimNoLegsSkin.e");

    
    // visualiseSeedPoints(filename, H);
    tetrahedraliseVacuumRegion(filename, outputFile, seed_points);
}
