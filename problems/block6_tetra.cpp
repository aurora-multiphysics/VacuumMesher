#include<iostream>
#include"tetMaker.hpp"
#include"visualiseSeedPoints.hpp"

int main(int argc, char** argv)
{   
    std::string path = "../Meshes/chimBlocks/skinnedBlocks/withBound/";
    std::string filename = "block6withBound.off";
    std::string outputExtension = ".mesh";
    std::string token = filename.substr(0, filename.find("."));
    std::string outputFile = path + "tetrahedralised/" + token + "_out" + outputExtension;
    filename = path + filename;
    std::cout << outputFile << std::endl;

    Eigen::MatrixXd H(1, 3);
    H << 
    // -0.37,-0.109553,0.338237; //top bit
    // 0.41, -0.011182, -0.325672, //bottom bit
    0.124686, -0.059741, -0.281648; //
     

     
    // visualiseSeedPoints(filename, H); 
    tetrahedraliseVacuumRegion(filename, outputFile, H);
}
