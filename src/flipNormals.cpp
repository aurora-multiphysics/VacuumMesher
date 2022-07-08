#include "flipNormals.hpp"

igl::opengl::glfw::Viewer viewer;
std::vector<Eigen::VectorXi> C(2);
std::vector<Eigen::MatrixXd> RGBcolors(2);
bool is_showing_reoriented = false;
bool facetwise = false;
Eigen::MatrixXd V;
Eigen::MatrixXi F;  
Eigen::MatrixXi T;
Eigen::VectorXi I;
std::vector<Eigen::MatrixXi> FF(2);

void flipNormals(std::string filename)
{
    using namespace std;


    igl::readMESH(filename, V, T, F);

    // Print the vertices and faces matrices
    // std::cout << "Vertices: " << std::endl << V << std::endl;
    // std::cout << "Faces:    " << std::endl << F << std::endl;


    const auto & scramble_colors = []()
    {
        for(int pass = 0;pass<2;pass++)
        {
            Eigen::MatrixXi R;
            igl::randperm(C[pass].maxCoeff()+1,R);
            C[pass] = igl::slice(R,Eigen::MatrixXi(C[pass]));
            Eigen::MatrixXd HSV(C[pass].rows(),3);
            HSV.col(0) = 
                360.*C[pass].array().cast<double>()/(double)C[pass].maxCoeff();
            HSV.rightCols(2).setConstant(1.0);
            igl::hsv_to_rgb(HSV,RGBcolors[pass]);
        }
        viewer.data().set_colors(RGBcolors[facetwise]);
    };

    viewer.callback_key_pressed = 
    [&scramble_colors]
    (igl::opengl::glfw::Viewer& /*viewer*/, unsigned int key, int mod)->bool
    {
        switch(key)
        {
        default:
        return false;
        case 'F':
        case 'f':
        {
        facetwise = !facetwise;
        break;
        }
        case 'S':
        case 's':
        {
        scramble_colors();
        return true;
        }
        case ' ':
        {
        is_showing_reoriented = !is_showing_reoriented;
        break;
        }
        }
        viewer.data().clear();
        viewer.data().set_mesh(V,is_showing_reoriented?FF[facetwise]:F);
        viewer.data().set_colors(RGBcolors[facetwise]);
        return true;
    };

    for(int pass = 0;pass<2;pass++)
    {
        Eigen::VectorXi I;
        igl::embree::reorient_facets_raycast(
        V,F,F.rows()*100,10,pass==1,false,false,I,C[pass]);
        // apply reorientation
        FF[pass].conservativeResize(F.rows(),F.cols());
        for(int i = 0;i<I.rows();i++)
        {
            if(I(i))
            {
                FF[pass].row(i) = (F.row(i).reverse()).eval();
            }else
            {
                FF[pass].row(i) = F.row(i);
            }
        }
    }
    viewer.data().set_mesh(V,is_showing_reoriented?FF[facetwise]:F);
    viewer.data().set_face_based(true);
    scramble_colors();
    viewer.launch();
}