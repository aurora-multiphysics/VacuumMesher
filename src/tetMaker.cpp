#include<tetMaker.hpp>

namespace fs = std::filesystem;

void tetrahedraliseVacuumRegion(std::string filename, std::string outname, Eigen::MatrixXd& seedPoints, libMesh::Mesh& vacuumMesh)
{   
    std::cout << filename << std::endl;
    std::string dir = std::filesystem::path(filename).parent_path().string() + "/";
    std::string stem = std::filesystem::path(filename).stem().string();
    std::string tetStem = std::filesystem::path(outname).stem().string();
    std::string offFilepath = dir + stem + ".off";
    std::string tetFilepath = dir + tetStem + ".mesh";

    convertMesh(filename, "off");
    Eigen::MatrixXd V;
    Eigen::MatrixXi F;  
    Eigen::MatrixXi T;
    igl::readOFF(offFilepath, V, F);

    // visualiseSeedPoints(offFilepath, seedPoints);


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


    igl::writeMESH(tetFilepath, TV, TT, emptyFaces);

    
    convertMesh(tetFilepath, "e");

    //Deleting unnecessary filetypes
    unlink(tetFilepath.c_str());
    unlink(offFilepath.c_str());
    vacuumMesh.read(dir+tetStem+".e");
}


Eigen::MatrixXd getSeeds(libMesh::Mesh mesh)
{
    Eigen::MatrixXd V;
    Eigen::MatrixXi F;  

    libMeshToIGL(mesh, V, F);

    Eigen::MatrixXd N_faces;

    Eigen::MatrixXd seed_points(F.rows(), 3);

    igl::per_face_normals(V,F,N_faces);

    for(int i; i < N_faces.rows(); i++)
    {
        std::vector<double> seed(3, 0);
        std::vector<double> normal = {N_faces.row(i)[0], N_faces.row(i)[1], N_faces.row(i)[2]};
        std::vector<int> verts = {F.row(i)[0], F.row(i)[1], F.row(i)[2]};
        for(auto& vert: verts){
            seed[0]+= (V.row(vert)[0] - 1e-10*normal[0])/3; // x component of seed
            seed[1]+= (V.row(vert)[1]- 1e-10*normal[1])/3; // y component of seed
            seed[2]+= (V.row(vert)[2]- 1e-10*normal[2])/3; // z component of seed
        }
        seed_points.row(i) << seed[0], seed[1], seed[2];
    }

    return seed_points;
}