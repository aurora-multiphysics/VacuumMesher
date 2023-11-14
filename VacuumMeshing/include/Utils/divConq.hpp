/** \class
 * Once a vacuum region mesh has been generated, it needs to be recombined,
 * with the mesh of the original part to create the full mesh. This will results
 * in lots of duplicate nodes. We can store nodes in an rTree, and when we want
 * to add nodes we query the tree to see if an identical node exists. To decide
 * whether the node is a duplicate or not, a tolerance value is used. So if
 * nodes are within the euclidian distance described by "tolerance", the rTree
 * returns a "hit" to show that an identical node already exists.
 *
 * The functionality in this class is used to help identify a suitable
 * tolerance. A divide and conquer algorithm is used to identify the closest
 * pair of nodes in a mesh. Using this value, we can identify a suitable choice
 * of tolerance for the rTree. To learn more about the divide and conquer check
 * out this paper by Shamos and Bentley,
 * http://euro.ecom.cmu.edu/people/faculty/mshamos/1976ShamosBentley.pdf
 */

#include "libmesh/libmesh.h"
#include "libmesh/mesh.h"
#include "libmesh/node.h"
#include "stdlib.h"
#include "string"

namespace VacuumMesher {
/** Class used to find the closest pair of nodes. Currently the rest of the
library is more functional than oop, but I think I will change this in future so
that it is all consistent. Because of this I made this a class, as I think
everything else will go that way eventually.*/

enum AXIS { X_AXIS, Y_AXIS, Z_AXIS };

class ClosestPairFinder {
public:
  // Default constructor
  ClosestPairFinder();

  // Constructor that should be used most of the time.
  ClosestPairFinder(libMesh::Mesh &mesh_);

  // Return the euclidian distance between the two closest points
  double closestPairMagnitude(int dim = 3);

protected:
  // Type def to prevent headache
  typedef std::pair<libMesh::Node *, libMesh::Node *> nodePair;

  //
  double closestPair2D(std::vector<libMesh::Node *> &X);

  /** Method to find the closest distance between any pair of points in a 3D
   * mesh. Uses divide and conquer to perform the operation in O(nlog(n)) time
   * (almost). A quick explanation can be given here, although I'm probably not
   * the best person to give it. The problem space, which can be thought of as a
   * points cloud, is split in
   * 2. The splitting point might not be the geometric middle, but there are an
   * approximately equal number of points on both sides. This is done
   * recursively for each subspace until the base cases are reached. These base
   * cases occur when n(number of points) = 2 or 3, at which point the distance
   * of these points is compared. When you find you have to combine two space,
   * algorithmic voodoo takes place to do this in not much time. This probably
   * isn't the place to learn or understand this, so follow the link to the
   * paper I put which does a much better job than I ever could.
   *
   * The only argument this function takes once arg, a vector of libMesh node
   * pointers, \p X. This vector should be pre sorted based on the x coords of
   * the nodes, from low to high.
   */
  double closestPair3D(std::vector<libMesh::Node *> &X);

  /** Method to return list of pairs of nodes that need their distance checking
   * in the 3D method.*/
  void getPotentialPairs(std::vector<libMesh::Node *> &X, double delta,
                         std::vector<nodePair> &pairs);

  /** Method to find the euclidian distance between \p n1 and \p n2 */
  double eucDist(libMesh::Node *n1, libMesh::Node *n2);

  /** Method that sorts \p vec by its x, y or z coordainte. \p axis = 0 will
   * sort by x, 1 by y and 2 by z*/
  void sortByIthCoord(AXIS axis, std::vector<libMesh::Node *> &vec);

  /** Method to intialise class member \p xPoints. xPoints is a vector
   * of libMesh node pointers, sorted by their x coordinate value, from low to
   * high
   */
  void initialiseArrays();

  /** Vector of libmesh node pointers you can use for the closestPair methods.
   * You might be wondering why the methods take an argument if they could just
   * directly call this vector, as it is a class member. This is not done
   * because the methods get called recursively, and need their own input vector
   * in each recursive call. If they just all reference \p xPoints stuff would
   * go wrong. This is here just out of convenience*/
  std::vector<libMesh::Node *> xPoints;

  /** shared_ptr to libmesh mesh, of which we want to find the closest pair of
   * nodes (points).*/
  std::shared_ptr<libMesh::Mesh> mesh;
};

} // namespace VacuumMesher
