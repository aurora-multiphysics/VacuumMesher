
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

    mesh.read(filepath);
    Eigen::MatrixXd surf_verts, bound_verts;
    Eigen::MatrixXi surf_faces, bound_faces;
    auto start1 = std::chrono::steady_clock::now();
    // Multimap to store which sides of the elements are boundary sides (i.e. which sides have the null neighbor)
    const double tol = 1e-05;

    std::multimap<unsigned int, unsigned int> surfaceFaceMap;
    getSurface(mesh, surfMesh, surfaceFaceMap);
    libMeshToIGL(surfMesh, surf_verts, surf_faces);
    // long long int surfNodes = surfMesh.n_nodes() - sidesetMesh.n_nodes() + sidesetBoundMesh.n_nodes();
    // Get seed points
    Eigen::MatrixXd seed_points = getSeeds(surfMesh, 1e-04);
    
    // surfMesh.write("surfhuh.e");
    // Turn surfMesh into boundaryMEsh
    getBasisChangeMesh(mesh, bound_verts, bound_faces, 300, 20, 0);
    // boundMesh.write("boundhuh.e");
    
    combineMeshes(tol, bound_verts, bound_faces, surf_verts, surf_faces);
    
    // boundMesh.write("thisShouldWork.e");
    // Tetrahedralise everything
    tetrahedraliseVacuumRegion(bound_verts, bound_faces, vacuumMesh, seed_points);
    // // Set up rTree with specified tolerance
    // vacuumMesh.write("iscoilcorrect.e");

    // long long int totalNodes = mesh.n_nodes() + vacuumMesh.n_nodes();
    
    combineMeshes(tol, mesh, vacuumMesh, surfaceFaceMap);
    auto end1 = std::chrono::steady_clock::now();
    std::cout << "Elapsed time in milliseconds: "
    << std::chrono::duration_cast<std::chrono::milliseconds>(end1 - start1).count()
    << " ms" << std::endl;
    mesh.write(tetFilepath);
    // long long int zero = totalNodes - (mesh.n_nodes() + surfNodes);
    // std::cout << "Is this 0?: " << zero << std::endl;
    // std::cout << mesh.n_elem() << " " << mesh.n_nodes() << std::endl;

    return 0;
}