#include "genFullMesh.hpp"

int main(int argc, char** argv)
{
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
    tetFilename = filenameNoExt + "_tet.e";

    surfFilepath = path + surfFilename; 
    boundFilepath = path + boundFilename;
    tetFilepath = path + tetFilename; 
    
    libMesh::LibMeshInit init(libmeshArgv.size() - 1, libmeshArgv.data());
    //Create mesh object to store original model mesh
    libMesh::Mesh mesh(init.comm());
    //Create mesh object to store surface mesh
    // libMesh::Mesh surfaceMesh(init.comm());
    //Create mesh object to store vacuum mesh
    libMesh::Mesh vacuumMesh(init.comm());

    std::cout << "Reading geometry mesh...\n" << std::endl; 
    mesh.read(filepath);
    std::cout << "Geometry mesh read\n" << std::endl; 
    // surfaceMesh.read(surfFilepath);
    std::cout << "Reading vacuum mesh...\n" << std::endl; 
    vacuumMesh.read(tetFilepath);
    std::cout << "Vacuum mesh read\n" << std::endl; 
    createFullGeometry(mesh, vacuumMesh);

    return 0;
}