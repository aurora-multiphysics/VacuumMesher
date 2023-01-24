#include "Tetrahedralisation/removeDupeNodes.hpp"
#include "BoundaryGeneration/changeOfBase.hpp"
#include "Utils/parseFlags.hpp"
#include <chrono>

int main(int argc, const char** argv)
{
    inputFlags flags{parse_settings(argc, argv)};
    flags.setSwitches();
    // Setup simple argv for use with libMesh init
    std::string appName(argv[0]);
    std::vector<char*> libmeshArgv = {(char*)appName.data()};
    std::string path, filepath, filenameNoExt, surfFilename, boundFilename, tetFilename; 
    std::string surfFilepath, boundFilepath, tetFilepath; 
    std::cout << flags.tetSettings << std::endl;
    // Set up appropriate file names
    path = "./Meshes/";
    filepath = path + flags.infile.value();
    filenameNoExt = flags.infile.value().substr(0, flags.infile.value().find("."));
    std::cout << filepath << std::endl;
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

    // Read in the mesh given by input flags
    mesh.read(filepath);

    // If user has not specified the length of the boundary, then it is set here
    if(!flags.bound_len.has_value())
    {
        // create bounding box around mesh
        auto box = libMesh::MeshTools::create_bounding_box(mesh);
        
        // Use bounding points to determine a suitable boundary length
        flags.bound_len = 1.5*(box.max() - box.min()).norm();
    }
    // Eigen matrices to store mesh data for surface mesh and boundary mesh
    Eigen::MatrixXd surf_verts, bound_verts;
    Eigen::MatrixXi surf_faces, bound_faces;
  
    // Multimap to store which sides of the elements are boundary sides (i.e. which sides have the null neighbor)
    std::multimap<unsigned int, unsigned int> surfaceFaceMap;
    getSurface(mesh, surfMesh, surfaceFaceMap, flags.verbose);
    // Convert surface mesh to libIGL compatible data structures
    libMeshToIGL(surfMesh, surf_verts, surf_faces);
    // long long int surfNodes = surfMesh.n_nodes() - sidesetMesh.n_nodes() + sidesetBoundMesh.n_nodes();
    // Get seed points
    Eigen::MatrixXd seed_points = getSeeds(surfMesh, 1e-04);
    
    // Turn surfMesh into boundaryMesh
    getBasisChangeMesh(mesh, bound_verts, bound_faces, flags.bound_len.value(), flags.bound_subd.value(), flags.triSettings);

    // If verbose flag is set, output the boundaryMesh
    if(flags.verbose)
    {
        IGLToLibMesh(boundMesh, bound_verts, bound_faces);
    }
    
    // Tolerance for rTree combining of meshes
    const double tol = 1e-05;
    // Combine the boundary with the surface mesh to create a closed manifold we can use for tetrahedrelisation
    combineMeshes(tol, bound_verts, bound_faces, surf_verts, surf_faces);
    
    // Tetrahedralise everything
    tetrahedraliseVacuumRegion(bound_verts, bound_faces, vacuumMesh, seed_points, flags.tetSettings);

    // long long int totalNodes = mesh.n_nodes() + vacuumMesh.n_nodes();    
    combineMeshes(tol, mesh, vacuumMesh, surfaceFaceMap);

    // Write the mesh to either the value provided in the input flags, or a default filepath
    mesh.write(flags.outfile.value_or(tetFilepath));
    // long long int zero = totalNodes - (mesh.n_nodes() + surfNodes);
    // std::cout << "Is this 0?: " << zero << std::endl;
    // std::cout << mesh.n_elem() << " " << mesh.n_nodes() << std::endl;

    return 0;
}