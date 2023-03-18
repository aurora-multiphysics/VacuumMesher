#include "BoundaryGeneration/boundaryGeneration.hpp"
#include "MeshContainer.hpp"
#include "SurfaceMeshing/surfaceMeshing.hpp"
#include "Tetrahedralisation/removeDupeNodes.hpp"
#include "Utils/parseFlags.hpp"
#include "Utils/divConq.hpp"
#include <chrono>

// Global tolerance for rTree merges

int main(int argc, const char** argv){

    inputFlags flags(parse_settings(argc, argv));
    flags.setSwitches();

    // Setup simple argv for use with libMesh init
    std::string appName(argv[0]);
    std::vector<char *> libmeshArgv = {(char *)appName.data()};
    // std::cout << flags.tetSettings << std::endl;

    // Initialise libmesh functions and mpi
    libMesh::LibMeshInit init(libmeshArgv.size() - 1, libmeshArgv.data());
    // Mesh container object, that has ownership of the mesh, surfaceMesh, Vacuum
    MeshContainer meshes(init, flags.infile.value());

    ClosestPairFinder pairFinder(meshes.userMesh().libmeshMesh());
    double tol = (pairFinder.closestPair3D(pairFinder.xPoints))/100;
    // Generate skinned mesh
    getSurface(meshes.userMesh().libmeshMesh(), meshes.skinnedMesh().libmeshMesh(), &(meshes.surfaceFaceMap()), true, meshes.skinFilename_);

    // Get seed points for tetrahedralisation 
    Eigen::MatrixXd seed_points = getSeeds(meshes.skinnedMesh().libmeshMesh());

    // long long int surfNodes = meshes.skinnedMesh().libmeshMesh().n_nodes();
    // Adds a boundary to the surface mesh
    addBoundary(meshes.skinnedMesh().libmeshMesh(), meshes.boundaryMesh().libmeshMesh(), flags.boundLen.value(),
              flags.boundSubd.value(), tol, flags.triSettings);

    meshes.boundaryMesh().libmeshMesh().write(meshes.boundFilename_);

    std::cout << flags.tetSettings << std::endl;
    // Tetrahedralise everything
    tetrahedraliseVacuumRegion(meshes.boundaryMesh().libmeshMesh(),
                               meshes.vacuumMesh().libmeshMesh(), seed_points, flags.tetSettings);

    

    auto t1 = std::chrono::high_resolution_clock::now();
    
    // Combine the vacuum mesh and the part mesh 
    combineMeshes(tol, meshes.userMesh().libmeshMesh(),
                  meshes.vacuumMesh().libmeshMesh(),
                  meshes.surfaceFaceMap());

    auto t2 = std::chrono::high_resolution_clock::now();
    auto t = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);   
    std::cout << "Tol = " << tol << ", combine time = " << t.count() << std::endl;
    // auto end1 = std::chrono::steady_clock::now();
    // std::cout << "Elapsed time in milliseconds: "
    // << std::chrono::duration_cast<std::chrono::milliseconds>(end1 - start1).count()
    // << " ms" << std::endl;
    // long long int zero = totalNodes - (mesh.n_nodes() + surfNodes);
    // std::cout << "Is this 0?: " << zero << std::endl;
    // std::cout << meshes.vacuumFilename_ << std::endl;
    meshes.userMesh().libmeshMesh().write(meshes.vacuumFilename_);

    return 0;
}