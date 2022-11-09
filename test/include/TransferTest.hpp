#pragma once
#include "MeshTest.hpp"
#include "libmeshToIgl.hpp"

class TransferTest : public MeshTest
{

public:
    
protected:
    typedef CGAL::Exact_predicates_exact_constructions_kernel K;
    typedef typename CGAL::Polyhedron_3<K, CGAL::Polyhedron_items_with_id_3> Polyhedron;
    typedef typename CGAL::Nef_polyhedron_3<K> Nef_Polyhedron;

    TransferTest(std::string meshFile) : MeshTest(meshFile)
    {
    }

    virtual void SetUp() override
    {
        getFilePaths();
        setMesh();
    }

    virtual void setMesh() 
    {   
        MeshTest::setMesh();
        refMesh = std::make_shared<libMesh::Mesh>(init->comm());
    }

    //Libmesh Mesh object
    std::shared_ptr<libMesh::Mesh> refMesh = nullptr;  
};