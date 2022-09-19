#include<iostream>
#include"tetMaker.hpp"

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
     -0.471954,-0.001158,0.81,
     -0.286693, 0.005973, 0.816620,
     -0.176693, 0.00, 0.789049;
     
    tetrahedraliseVacuumRegion(filename, outputFile, H);
}
