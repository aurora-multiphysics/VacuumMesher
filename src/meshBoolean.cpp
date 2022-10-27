#include"meshBoolean.hpp"

// namespace fs = std::filesystem;

void genBooleanBound(std::string& boundPath, std::string& surfacePath, 
                     libMesh::Mesh& boundaryMesh, libMesh::Mesh& surfaceMesh)
{   
    std::string boundDir = std::filesystem::path(boundPath).parent_path().string() + "/";
    std::string boundStem = std::filesystem::path(boundPath).stem().string();
    std::string surfDir = std::filesystem::path(surfacePath).parent_path().string() + "/";
    std::string surfStem = std::filesystem::path(surfacePath).stem().string();
    std::string boundOffPath = boundDir + boundStem + ".off"; 
    std::string surfOffPath = surfDir + surfStem + ".off"; 
    std::string boundOutOffPath = boundDir + boundStem + "_out.off"; 

    convertMesh(boundPath, "off");
    convertMesh(surfacePath, "off");
    std::cout << surfOffPath << " " << boundOffPath << std::endl;
    Eigen::MatrixXd VA,VB,VC;
    Eigen::MatrixXi FA, FB, FC;  
    Eigen::VectorXi J,I;
    std::cout << boundPath << std::endl;
    igl::readOFF(surfOffPath, VA, FA);
    igl::readOFF(boundOffPath, VB, FB);
    std::cout << VA.rows() << " " << VB.rows() << std::endl;
    igl::MeshBooleanType boolean_type(igl::MESH_BOOLEAN_TYPE_XOR);

    igl::copyleft::cgal::mesh_boolean(VA,FA,VB,FB,boolean_type,VC,FC,J);
    igl::writeOFF(boundOutOffPath, VC, FC);
    // igl::writeOFF("./testingworkingsurface.off", VB, FB);



    
    convertMesh(boundOutOffPath, "e");
    // convertMesh("./testingworkingsurface.off", "e");

    //Deleting unnecessary filetypes
    // unlink(tetFilepath.c_str());
    // unlink(offFilepath.c_str());
}
