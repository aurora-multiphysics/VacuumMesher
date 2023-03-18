#include "Tetrahedralisation/removeDupeNodes.hpp"
#include <chrono>
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
    libMesh::Mesh surfMesh(init.comm());
    //Create mesh object to store vacuum mesh
    libMesh::Mesh vacuumMesh(init.comm());

    auto start1 = std::chrono::steady_clock::now();
    mesh.read(filepath);
    surfMesh.clear();

    // Multimap to store which sides of the elements are boundary sides (i.e. which sides have the null neighbor)
    std::multimap<unsigned int, unsigned int> surfaceFaceMap;
    getSurface(mesh, surfMesh, surfaceFaceMap, true, surfFilepath);
    // Get seed points for tetrahedralisation 
    // surfMesh.read("./Meshes/target_surf_cubit.e");
    Eigen::MatrixXd seed_points = getSeeds(surfMesh);

    long long int surfNodes = surfMesh.n_nodes();
    // Adds a boundary to the surface mesh
    createBoundary(init, surfMesh, 1.2);

    // Tetrahedralise everything
    tetrahedraliseVacuumRegion(surfMesh, vacuumMesh, seed_points);
    long long int totalNodes = mesh.n_nodes() + vacuumMesh.n_nodes();
    
    vacuumMesh.write("targetVac.e");
    // Combine the vacuum mesh and the part mesh 
    combineMeshes(1e-05, mesh, vacuumMesh, surfaceFaceMap);
    auto end1 = std::chrono::steady_clock::now();
    std::cout << "Elapsed time in milliseconds: "
    << std::chrono::duration_cast<std::chrono::milliseconds>(end1 - start1).count()
    << " ms" << std::endl;
    long long int zero = totalNodes - (mesh.n_nodes() + surfNodes);
    std::cout << "Is this 0?: " << zero << std::endl;
    mesh.write(tetFilepath);

    return 0;
}