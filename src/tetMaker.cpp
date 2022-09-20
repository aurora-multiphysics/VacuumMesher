#include<tetMaker.hpp>

void tetrahedraliseVacuumRegion(std::string filename, std::string outname, Eigen::MatrixXd seedPoints)
{   
    Eigen::MatrixXd V;
    Eigen::MatrixXi F;  
    Eigen::MatrixXi T;
    igl::readOFF(filename, V, F);
    // igl::opengl::glfw::Viewer viewer;
    // viewer.data().set_mesh(V, F);
    // viewer.data().add_points(H, Eigen::RowVector3d(1,0,0));
    // viewer.launch();

    Eigen::MatrixXd R;
    Eigen::MatrixXd TV;
    Eigen::MatrixXi TT;  
    Eigen::MatrixXi TF;
    Eigen::MatrixXd TR;  
    Eigen::MatrixXi TN;
    Eigen::MatrixXi PT;  
    Eigen::MatrixXi FT;
    size_t numRegions;
    
    igl::copyleft::tetgen::tetrahedralize(V, F, seedPoints, R, "pqCVY", TV, TT, TF, TR, TN, PT, FT, numRegions);
    
    //Empty matrix. If we put TF as an argument instead, writeMESH will also output the original skinned
    // mesh as a block. This results in hissy fits if you try and skin the mesh again, as the tri facets
    // from the extra block overlap with the faces of the tets and everything goes haywire
    Eigen::MatrixXi emptyFaces;
    igl::writeMESH(outname, TV, TT, emptyFaces);
}
