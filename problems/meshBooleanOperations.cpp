#include "BoundaryGeneration/generateBoundaries.hpp"
#include "Tetrahedralisation/tetMaker.hpp"
#include "BoundaryGeneration/meshBoolean.hpp"
#include "Tetrahedralisation/removeDupeNodes.hpp"

#include <igl/copyleft/cgal/remesh_intersections.h>

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
    boundFilename = filenameNoExt + "_bound_only.e";
    tetFilename = filenameNoExt + "_tet.e";

    surfFilepath = path + surfFilename; 
    boundFilepath = path + boundFilename;
    tetFilepath = path + tetFilename; 

    // Multimap to store which sides of the elements are boundary sides (i.e. which sides have the null neighbor)
    std::multimap<unsigned int, unsigned int> surfaceFaceMap;

    libMesh::LibMeshInit init(libmeshArgv.size() - 1, libmeshArgv.data());
    //Create mesh object to store original model mesh
    libMesh::Mesh mesh(init.comm());
    //Create mesh object to store surface mesh
    libMesh::Mesh surfaceMesh(init.comm());
    //Create mesh object to store vacuum mesh
    libMesh::Mesh vacuumMesh(init.comm());
    libMesh::Mesh boundaryMesh(init.comm());



    surfaceMesh.read(surfFilepath);

    boundaryMesh.read(boundFilepath);
    // Puts combined mesh into 
    std::cout << "genning boolean bound" << std::endl;
    genBooleanBound(boundaryMesh, surfaceMesh);
    boundaryMesh.write("booleanBoxTest.e");
    std::cout << "gennED boolean bound" << std::endl;
    // tetrahedraliseVacuumRegion(boundaryMesh, vacuumMesh);
    // vacuumMesh.write("vacuumMeshCoilTest.e");
    // createEdgeMesh(surfaceMesh, boundaryMesh);

    // vacuumMesh.read(tetFilepath);

    // combineMesh(mesh, vacuumMesh);

    return 0;
}