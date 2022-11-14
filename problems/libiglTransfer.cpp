#include "libmeshToIgl.hpp"

int main(int argc, char** argv)
{   
    // 0. Set up libmesh environment and meshes
    // try
    // {   
    //     if(argc == 1)
    //     {
    //         throw 0;
    //     }
    // } catch(...)
    // {
    //     std::cout << "Error, mesh file argument is required." << std::endl;
    //     return -1;
    // }
    std::string appName(argv[0]);

    std::vector<char*> libmeshArgv = {(char*)appName.data()};
    std::string path, filepath, filenameNoExt, surfFilename, boundFilename, tetFilename; 
    std::string surfFilepath, boundFilepath, tetFilepath; 
    std::string filename(argv[1]);

    path = "./Meshes/";
    filepath = path + filename;
    filenameNoExt = filename.substr(0, filename.find("."));

    surfFilename = filenameNoExt + "_surf.e";
    boundFilename = filenameNoExt + "_bound.e";
    tetFilename = filenameNoExt + "_tet.mesh";

    surfFilepath = path + surfFilename; 
    boundFilepath = path + boundFilename;
    tetFilepath = path + tetFilename; 
    
    libMesh::LibMeshInit init(libmeshArgv.size() - 1, libmeshArgv.data());
    //Create mesh object to store original model mesh
    libMesh::Mesh mesh(init.comm());
    //Create mesh object to store surface mesh
    libMesh::Mesh surfaceMesh(init.comm());
    //Create mesh object to store vacuum mesh
    libMesh::Mesh testMesh(init.comm());
    
    //Read volume mesh
    // std::cout << "Reading mesh...\n" << std::endl;
    // mesh.read(filepath);
    // std::cout << "Mesh read successfully" << std::endl;
    // 1. Produce Skinned Mesh 
    // Perhaps I need to make sure that the centroid of the shape is set to 0,0,0 at or 
    // BEFORE this point
    // Get the surface mesh
    surfaceMesh.read(surfFilepath);

    Eigen::MatrixXd V;
    Eigen::MatrixXi F;

    libMeshToIGL(surfaceMesh, V, F);
    IGLToLibMesh(testMesh, V, F);
    testMesh.write("testingIGL.e");

    return 0;
}