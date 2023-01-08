
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
    libMesh::Mesh coilMesh(init.comm());
    //Create mesh object to store surface mesh
    libMesh::Mesh targetMesh(init.comm());
    // 
    libMesh::Mesh targetSurfMesh(init.comm());

    // 
    libMesh::Mesh boundaryMesh(init.comm());
    //Create mesh object to store vacuum mesh
    libMesh::Mesh vacuumMesh(init.comm());

    auto start1 = std::chrono::steady_clock::now();
    coilMesh.read(filepath);
    boundaryMesh.read("./Meshes/hive_coil_boundary.e");
    targetMesh.read("./Meshes/target.e");
    // vacuumMesh.read(tetFilepath);
    // Multimap to store which sides of the elements are boundary sides (i.e. which sides have the null neighbor)
    std::multimap<unsigned int, unsigned int> surfaceFaceMap;
    getSurface(targetMesh, targetSurfMesh, surfaceFaceMap, true, surfFilepath);
    // Get seed points for tetrahedralisation 
    Eigen::MatrixXd seed_points = getSeeds(targetSurfMesh);

    // Adds a boundary to the coil mesh, that is coincident with the xy plane (z=0)
    // createCoilBoundary(init, surfMesh, 2);

    // Tetrahedralise everything
    tetrahedraliseVacuumRegion(boundaryMesh, vacuumMesh, seed_points);
    vacuumMesh.write("hive_vac.e");
    // // Set up rTree with specified tolerance
    // double tol = 1e-07;

    // combineMesh(tol, mesh, vacuumMesh, surfaceFaceMap);
    // auto end1 = std::chrono::steady_clock::now();
    // std::cout << "Elapsed time in milliseconds: "
    // << std::chrono::duration_cast<std::chrono::milliseconds>(end1 - start1).count()
    // << " ms" << std::endl;
    // vacuumMesh.write(tetFilepath);

    return 0;
}