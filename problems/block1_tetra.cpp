#include<iostream>
#include"tetMaker.hpp"
#include"visualiseSeedPoints.hpp"

int main(int argc, char** argv)
{   
    std::string path = "../Meshes/chimBlocks/skinnedBlocks/withBound/";
    std::string filename = "block1withBound.off";
    std::string outputExtension = ".mesh";
    std::string token = filename.substr(0, filename.find("."));
    std::string outputFile = path + "tetrahedralised/" + token + "_out" + outputExtension;
    filename = path + filename;
    std::cout << outputFile << std::endl;

    Eigen::MatrixXd H(3, 3);
    H << 
     -0.53193,0.0,-0.46,
     -0.286693, 0.005973, 0.816620,
     -0.176693, 0.00, 0.789049;
     
    // visualiseSeedPoints(filename, H);
    tetrahedraliseVacuumRegion(filename, outputFile, H);
}
