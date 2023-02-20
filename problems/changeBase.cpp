#include "Tetrahedralisation/removeDupeNodes.hpp"
#include "BoundaryGeneration/changeOfBase.hpp"
#include "Utils/parseFlags.hpp"
#include "Utils/MeshContainer.hpp"
#include <chrono>

int main(int argc, const char** argv)
{
    inputFlags flags{parse_settings(argc, argv)};
    flags.setSwitches();
    // Setup simple argv for use with libMesh init
    std::string appName(argv[0]);
    std::vector<char*> libmeshArgv = {(char*)appName.data()};
    std::cout << flags.tetSettings << std::endl;
    // Set up appropriate file names
    
    libMesh::LibMeshInit init(libmeshArgv.size() - 1, libmeshArgv.data());
    //Create mesh object to store original model mesh
    MeshContainer meshes(init, flags.infile.value());

    // If user has not specified the length of the boundary, then it is set here
    if(!flags.bound_len.has_value())
    {
        // create bounding box around mesh
        auto box = libMesh::MeshTools::create_bounding_box(meshes.userMesh().libmeshMesh());
        
        // Use bounding points to determine a suitable boundary length
        flags.bound_len = 1.5*(box.max() - box.min()).norm();
    }
  
    // Multimap to store which sides of the elements are boundary sides (i.e. which sides have the null neighbor)
    getSurface(meshes.userMesh().libmeshMesh(),
               meshes.skinnedMesh().libmeshMesh(),
               meshes.surfaceFaceMap(),
               flags.verbose);

    // Convert surface mesh to libIGL compatible data structures
    meshes.skinnedMesh().createIglAnalogue();

    // Get seed points
    Eigen::MatrixXd seed_points = getSeeds(meshes.skinnedMesh().libmeshMesh(), 1e-04);
    
    // Turn surfMesh into boundaryMesh
    getBasisChangeMesh(meshes.userMesh().libmeshMesh(),
                       meshes.boundaryMesh().iglVerts(),
                       meshes.boundaryMesh().iglElems(),
                       flags.bound_len.value(),
                       flags.bound_subd.value(),
                       flags.triSettings);

    // If verbose flag is set, output the boundaryMesh
    if(flags.verbose)
    {
        meshes.boundaryMesh().createLibmeshAnalogue();
        meshes.boundaryMesh().libmeshMesh().write(meshes.boundFilename_);
    }
    
    // Tolerance for rTree combining of meshes
    const double tol = 1e-05;

    // Combine the boundary with the surface mesh to create a closed manifold we can use for tetrahedrelisation
    combineMeshes(tol, 
                meshes.boundaryMesh().iglVerts(),
                meshes.boundaryMesh().iglElems(),
                meshes.skinnedMesh().iglVerts(),
                meshes.skinnedMesh().iglElems());
    
    // Tetrahedralise everything
    tetrahedraliseVacuumRegion(meshes.boundaryMesh().iglVerts(),
                               meshes.boundaryMesh().iglElems(),
                               meshes.vacuumMesh().libmeshMesh(),
                               seed_points,
                               flags.tetSettings);

    // 
    combineMeshes(tol,
                  meshes.userMesh().libmeshMesh(),
                  meshes.vacuumMesh().libmeshMesh(),
                  meshes.surfaceFaceMap());
    
    meshes.userMesh().libmeshMesh().write("neatnessTest.e");
    // // Write the mesh to either the value provided in the input flags, or a default filepath

    return 0;
}