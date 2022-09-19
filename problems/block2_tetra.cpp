#include<iostream>
#include"tetMaker.hpp"

int main(int argc, char** argv)
{   
    std::string path = "../Meshes/chimBlocks/skinnedBlocks/withBound/";
    std::string filename = "block2withBound.off";
    std::string outputExtension = ".mesh";
    std::string token = filename.substr(0, filename.find("."));
    std::string outputFile = path + "tetrahedralised/" + token + "_out" + outputExtension;
    filename = path + filename;
    std::cout << outputFile << std::endl;

    Eigen::MatrixXd H(1, 3);
    H << 
     -0.705,0.413001,1.284344;
   
     
    tetrahedraliseVacuumRegion(filename, outputFile, H);
}
