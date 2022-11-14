#include<iostream>
#include"tetMaker.hpp"

int main(int argc, char** argv)
{   
    std::string path = "../Meshes/chimBlocks/skinnedBlocks/withBound/";
    std::string filename = "block5withBound.off";
    std::string outputExtension = ".mesh";
    std::string token = filename.substr(0, filename.find("."));
    std::string outputFile = path + "tetrahedralised/" + token + "_out" + outputExtension;
    filename = path + filename;
    std::cout << outputFile << std::endl;

    Eigen::MatrixXd H(4, 3);
    H << 
     0.016825,-0.005613,-0.610000,
     0.016825,-0.005613,-0.180131,
     0.016825,-0.005613,0.150000,
     0.016825,-0.005613,0.579811;
  

    tetrahedraliseVacuumRegion(filename, outputFile, H);
}
