#include<iostream>
#include"tetMaker.hpp"
#include"visualiseSeedPoints.hpp"

int main(int argc, char** argv)
{   
    std::string path = "../Meshes/chimBlocks/skinnedBlocks/withBound/";
    std::string filename = "chimNoLegsBound.off";
    std::string outputExtension = ".mesh";
    std::string token = filename.substr(0, filename.find("."));
    std::string outputFile = path + "tetrahedralised/" + token + "_out" + outputExtension;
    filename = path + filename;
    std::cout << outputFile << std::endl;

    Eigen::MatrixXd H(10, 3);
    H << 
     -0.53193,0.0,-0.46, //Block1
     -0.286693, 0.005973, 0.816620,
     -0.176693, 0.00, 0.789049,
     -0.705,0.413001,1.284344, //Block 2
     0.016825,-0.005613,-0.610000, //Block 5
     0.016825,-0.005613,-0.180131,
     0.016825,-0.005613,0.150000,
     0.016825,-0.005613,0.579811,
     -0.37,-0.109553,0.338237, //Block 6
     0.124686, -0.059741, -0.281648; 


     
    // visualiseSeedPoints(filename, H);
    tetrahedraliseVacuumRegion(filename, outputFile, H);
}
