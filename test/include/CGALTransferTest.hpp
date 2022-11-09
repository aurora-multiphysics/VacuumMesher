#pragma once
#include "TransferTest.hpp"

class CGALTransferTest : public TransferTest
{
    typedef CGAL::Exact_predicates_exact_constructions_kernel K;
    typedef typename CGAL::Polyhedron_3<K, CGAL::Polyhedron_items_with_id_3> Polyhedron;
    typedef typename CGAL::Nef_polyhedron_3<K> Nef_Polyhedron;
public:
    
protected:
    

    CGALTransferTest(std::string meshFile) : TransferTest(meshFile)
    {
    }

    virtual void SetUp() override
    {
        getFilePaths();
        setMesh();
    }

    virtual void setMesh() 
    {   
        TransferTest::setMesh();
        mesh = std::make_shared<libMesh::Mesh>(init->comm());
        readMesh(mesh, refSurfaceFilepath);
        Polyhedron poly;
        libmeshToCGAL(*mesh, poly);
        CGALToLibmesh(*refMesh, poly);
    }
};