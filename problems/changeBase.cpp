
#include "Tetrahedralisation/removeDupeNodes.hpp"
#include "BoundaryGeneration/changeOfBase.hpp"
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

    libMesh::Mesh sidesetMesh(init.comm());
    libMesh::Mesh sidesetBoundMesh(init.comm());

    //Create mesh object to store surface mesh
    libMesh::Mesh surfMesh(init.comm());
    // 
    libMesh::Mesh boundMesh(init.comm());
    //Create mesh object to store vacuum mesh
    libMesh::Mesh vacuumMesh(init.comm());

    auto start1 = std::chrono::steady_clock::now();
    mesh.read(filepath);

    std::set<libMesh::boundary_id_type> ids;
    ids.insert(1);
    ids.insert(2);
    mesh.get_boundary_info().sync(ids, sidesetMesh);

    // Multimap to store which sides of the elements are boundary sides (i.e. which sides have the null neighbor)
    
    // getSurface(sidesetMesh, sidesetBoundMesh);

    std::multimap<unsigned int, unsigned int> surfaceFaceMap;
    getSurface(mesh, surfMesh, surfaceFaceMap);
    // Get seed points
    Eigen::MatrixXd seed_points = getSeeds(surfMesh);
    // Turn surfMesh into boundaryMEsh
    // getBasisChangeMesh(surfMesh, sidesetBoundMesh, boundMesh);
    boundMesh.read(boundFilepath);
     
    // Tetrahedralise everything
    tetrahedraliseVacuumRegion(boundMesh, vacuumMesh, seed_points);
    // // Set up rTree with specified tolerance

    // combineMeshes(1e-07, mesh, vacuumMesh, surfaceFaceMap);
    // auto end1 = std::chrono::steady_clock::now();
    // std::cout << "Elapsed time in milliseconds: "
    // << std::chrono::duration_cast<std::chrono::milliseconds>(end1 - start1).count()
    // << " ms" << std::endl;
    // mesh.write("hiveTarget.e");
    vacuumMesh.write(tetFilepath);

    return 0;
}