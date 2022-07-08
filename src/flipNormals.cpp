#include "flipNormals.hpp"

void flipNormals(std::string filename)
{
    Eigen::MatrixXd V;
    Eigen::MatrixXi F;  
    Eigen::MatrixXi T;
    Eigen::VectorXi I;
    std::vector<Eigen::MatrixXi> FF(2);

    igl::readMESH(filename, V, T, F);

    // Print the vertices and faces matrices
    std::cout << "Vertices: " << std::endl << V << std::endl;
    std::cout << "Faces:    " << std::endl << F << std::endl;

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
}