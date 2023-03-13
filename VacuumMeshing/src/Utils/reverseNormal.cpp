#include "Utils/reverseNormal.hpp"

void
reverseNormal(unsigned int facetID , Eigen::MatrixXi& F)
{
    unsigned int indexStore = F(facetID, 0);
    F(facetID, 0) = F(facetID, 2);
    F(facetID, 2) = indexStore;
}