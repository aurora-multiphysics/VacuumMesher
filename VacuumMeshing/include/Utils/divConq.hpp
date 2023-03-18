#include "stdlib.h"
#include "string"
#include "libmesh/libmesh.h"
#include "libmesh/mesh.h"
#include "libmesh/node.h"

class ClosestPairFinder
{
    typedef std::pair<libMesh::Node*, libMesh::Node*> nodePair;

 public:
    ClosestPairFinder();

    ClosestPairFinder(libMesh::Mesh& mesh);

    double closestPair2D(std::vector<libMesh::Node*> &X);

    double closestPair3D(std::vector<libMesh::Node*> &X);

    double closestPairHead(std::vector<libMesh::Node*> &X);

    // Comp axis assumes 2D problem, and that are comparison axis is Y (hence axis=1 because 0 indexed i.e. x=0 y=1 z=2)
    void enumeratePotentialPairs(std::vector<libMesh::Node*> &X, double delta, 
                                 std::vector<nodePair> &pairs,
                                 int compAxis=1);

    double dist(libMesh::Node* n1, libMesh::Node* n2);

    void sortByIthCoord(unsigned int axis, std::vector<libMesh::Node*>& vec);

    void initialiseArrays();

    std::vector<libMesh::Node*> xPoints, yPoints, zPoints;

    std::shared_ptr<libMesh::Mesh> mesh;  
};