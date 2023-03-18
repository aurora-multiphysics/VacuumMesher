#include "Utils/divConq.hpp"



ClosestPairFinder::ClosestPairFinder(libMesh::Mesh& mesh)
{
    this->mesh = std::make_shared<libMesh::Mesh>(mesh);
    initialiseArrays();
}

double ClosestPairFinder::closestPair2D(std::vector<libMesh::Node*> &X)
{
    // Create a copy of X called Y
    std::vector<libMesh::Node*> Y = X;

    // Sort Y by the y coordinate
    sortByIthCoord(1, Y);

    int n = X.size();
    // Base cases
    // if(n == 1){;}
    if(n == 2){return dist(X[0], X[1]);}
    if(n == 3){std::vector<double> dists{dist(X[0], X[1]), dist(X[0], X[2]), dist(X[1], X[2])}; return *std::min_element(dists.begin(), dists.end());}

    // std::vector<libMesh::Node*>::iterator midPoint = X[n/2]; 

    std::vector<libMesh::Node*> xS1(X.begin(), X.begin()+n/2);
    std::vector<libMesh::Node*> xS2(X.begin()+n/2, X.end());

    double midX = (**(X.begin()+n/2))(0);

    // Recursion
    double d1 = closestPair2D(xS1);
    double d2 = closestPair2D(xS2);

    double delta = std::min(d1, d2);

    // std::cout << "Recursion done" << std::endl;
    std::vector<libMesh::Node*> S;
    for(auto& node: Y)
    {
        if((midX-delta) <= (*node)(0) <= (midX+delta))
        {
            S.push_back(node);
        }
    }
    // std::cout << "Made Y" << std::endl;
    for(auto& node: S)
    {
        for(int i = 1; i<=7; i++)
        {
            (dist(node, node+i) < delta) ? delta = dist(node, node+i) : delta = delta;
        }
    }
    return delta;
}

double ClosestPairFinder::closestPair3D(std::vector<libMesh::Node*> &X)
{
    // Create a copy of X called Y
    std::vector<libMesh::Node*> Y = X;

    // Sort Y by the y coordinate
    sortByIthCoord(1, Y);

    int n = X.size();
    // Base cases
    // if(n == 1){;}
    if(n == 2){return dist(X[0], X[1]);}
    if(n == 3){std::vector<double> dists{dist(X[0], X[1]), dist(X[0], X[2]), dist(X[1], X[2])}; return *std::min_element(dists.begin(), dists.end());}

    // std::vector<libMesh::Node*>::iterator midPoint = X[n/2]; 

    std::vector<libMesh::Node*> xS1(X.begin(), X.begin()+n/2);
    std::vector<libMesh::Node*> xS2(X.begin()+n/2, X.end());

    double midX = (**(X.begin()+n/2))(0);
    // Recursion
    double d1 = closestPair3D(xS1);
    double d2 = closestPair3D(xS2);
    
    double delta = std::min(d1, d2);

    // std::cout << "Recursion done" << std::endl;
    std::vector<libMesh::Node*> slabSet;

    for(auto& node: Y)
    {
        if((midX-delta) <= (*node)(0) <= (midX+delta))
        {
            slabSet.push_back(node);
        }
    }
    // Incase there is nothing in the strip
    if(slabSet.size() == 0)
    {
        return delta;
    }

    std::vector<nodePair> pairs; 
    enumeratePotentialPairs(slabSet, delta, pairs, 2);

    // For potential pairs, test to see if they are closer than delta
    for(auto& pair: pairs)
    {
        (dist(pair.first, pair.second) < delta) ? delta = dist(pair.first, pair.second) : delta = delta;
    }

    return delta;
}

void ClosestPairFinder::enumeratePotentialPairs(std::vector<libMesh::Node*> &X, double delta, 
                                                std::vector<nodePair>& pairs,
                                                int compAxis)
{
    // Create a copy of X called Y
    std::vector<libMesh::Node*> Y = X;

    // Sort Y by the y coordinate
    sortByIthCoord(compAxis, Y);

    int n = X.size();
    // Base cases
    // if(n == 1){;}
    
    if(n == 2)
    {
        if(dist(X[0], X[1]) <= delta)
        {
            pairs.push_back(nodePair(X[0], X[1]));
        }
        return;
    }

    if(n == 3)
    {
        std::vector<double> dists{dist(X[0], X[1]), dist(X[0], X[2]), dist(X[1], X[2])}; 
        if(dist(X[0], X[1]) <= delta)
        {
            pairs.push_back(nodePair(X[0], X[1]));
        }

        if(dist(X[0], X[2]) <= delta)
        {
            pairs.push_back(nodePair(X[0], X[2]));
        }

        if(dist(X[1], X[2]) <= delta)
        {
            pairs.push_back(nodePair(X[1], X[2]));
        }
        return;
    }
    std::vector<libMesh::Node*> xS1(X.begin(), X.begin()+n/2);
    std::vector<libMesh::Node*> xS2(X.begin()+n/2, X.end());

    // Does not like this??
    double midX = (**(X.begin()+n/2))(0);


    // Recursion

    enumeratePotentialPairs(xS1, delta, pairs, compAxis);
    enumeratePotentialPairs(xS2, delta, pairs, compAxis);

    // std::cout << "Recursion done" << std::endl;
    std::vector<libMesh::Node*> S;
    for(auto& node: Y)
    {
        if((midX-delta) <= (*node)(0) <= (midX+delta))
        {
            S.push_back(node);
        }
    }
    // std::cout << "Made Y" << std::endl;
    for(auto& node: S)
    {
        for(int i = 1; i<=7; i++)
        {
            if(dist(node, node+i) <= delta)
            {
                pairs.push_back(nodePair(X[1], X[2]));
            } 
              
        }
    }
}

double ClosestPairFinder::closestPairHead(std::vector<libMesh::Node*> &X)
{
    std::vector<libMesh::Node*> Y = X;
    sortByIthCoord(1, Y);

    int n = X.size();
    // Base cases
    // if(n == 1){;}
    if(n == 2){return dist(X[0], X[1]);}
    if(n == 3){std::vector<double> dists{dist(X[0], X[1]), dist(X[0], X[2]), dist(X[1], X[2])}; return *std::min_element(dists.begin(), dists.end());}

    // std::vector<libMesh::Node*>::iterator midPoint = X[n/2]; 

    std::vector<libMesh::Node*> xS1(X.begin(), X.begin()+n/2);
    std::vector<libMesh::Node*> xS2(X.begin()+n/2, X.end());

    double midX = (**(X.begin()+n/2))(0);

    double d1 = closestPair2D(xS1);
    double d2 = closestPair2D(xS2);

    double delta = std::min(d1, d2);

    std::cout << "Recursion done" << std::endl;
    std::vector<libMesh::Node*> S;
    for(auto& node: Y)
    {
        if((midX-delta) <= (*node)(0) <= (midX+delta))
        {
            S.push_back(node);
        }
    }
    std::cout << "Made Y" << std::endl;
    for(auto& node: S)
    {
        for(int i = 1; i<=7; i++)
        {
            (dist(node, node+i) < delta) ? delta = dist(node, node+i) : delta = delta;
        }
    }
    return delta;
}

void ClosestPairFinder::sortByIthCoord(unsigned int axis, std::vector<libMesh::Node*>& vec)
{
    std::sort(vec.begin(), vec.end(), [axis](libMesh::Node* lhs, libMesh::Node* rhs) {
      return (*lhs)(axis) < (*rhs)(axis);
    });
}

void ClosestPairFinder::initialiseArrays()
{
    xPoints.reserve(mesh->n_nodes());

    // Put all nodes from mesh into xPoints vector
    for(auto node: mesh->node_ptr_range())
    {
        xPoints.emplace_back(node);
    }

    sortByIthCoord(0, xPoints);
}

double ClosestPairFinder::dist(libMesh::Node* n1, libMesh::Node* n2)
{
   return std::sqrt(std::pow(((*n2)(0) - (*n1)(0)), 2) + std::pow(((*n2)(1) - (*n1)(1)), 2) + std::pow(((*n2)(2) - (*n1)(2)), 2)); 
}