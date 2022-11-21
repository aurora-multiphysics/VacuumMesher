#include "removeDupeNodes.hpp"

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

    libMesh::Mesh surfMesh(init.comm());
    //Create mesh object to store vacuum mesh
    libMesh::Mesh vacuumMesh(init.comm());

    mesh.read(filepath);
    vacuumMesh.read(tetFilepath);
    surfMesh.read(surfFilepath);

    std::cout << mesh.n_nodes() << std::endl;
    std::cout << surfMesh.n_nodes() << std::endl;
    RTree<int, double, 3, float> rtree;
    double tol = 1e-07;
    createTree(rtree, vacuumMesh, tol);
    combineMesh(rtree, tol, mesh, vacuumMesh);
    rtree.RemoveAll();
    vacuumMesh.write("combMeshOut.e");

    return 0;
}