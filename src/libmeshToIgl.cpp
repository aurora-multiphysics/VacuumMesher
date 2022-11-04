#include "libmeshToIgl.hpp"




void 
libMeshToIGL(libMesh::Mesh& libmeshMesh, Eigen::MatrixXd& V, Eigen::MatrixXi& F, unsigned int dim)
{
    V = Eigen::MatrixXd(libmeshMesh.n_nodes(), dim);
    F = Eigen::MatrixXi(libmeshMesh.n_elem(), libmeshMesh.elem_ptr(0)->n_nodes());
    for(auto& node: libmeshMesh.local_node_ptr_range())
    {
        V(node->id(), 0) = (*node)(0);
        V(node->id(), 1) = (*node)(1);
        
        if(dim == 3)
        {
            V(node->id(), 2) = (*node)(2);
        }
    }

    for(auto& elem: libMesh::as_range(libmeshMesh.local_elements_begin(), libmeshMesh.local_elements_end()))
    {
        std::vector<libMesh::dof_id_type> connec;
        elem->connectivity(0, libMesh::IOPackage::VTK, connec);
        for(int i = 0; i<connec.size(); i++)
        {
            F(elem->id(), i) = connec[i];
        }
    }
}

void 
IGLToLibMesh(libMesh::Mesh& libmeshMesh, Eigen::MatrixXd& V, Eigen::MatrixXi& F)
{
    std::cout << V.rows() << std::endl;
    for(int nodeID = 0; nodeID < V.rows(); nodeID++)
    {
        double pnt[V.cols()];
        for(int i = 0; i<V.cols(); i++)
        {
            pnt[i] = V(nodeID, i);
        }

        libMesh::Point xyz(pnt[0], pnt[1], pnt [2]);
        libmeshMesh.add_point(xyz, nodeID);
    }
    for(int elemID = 0; elemID < F.rows(); elemID++)
    {
        libMesh::Elem* elem = libMesh::Elem::build(getElemType(F)).release();
        for(int j = 0; j < F.cols(); j++)
        {
            elem->set_node(j) = libmeshMesh.node_ptr(F(elemID, j));
        }
        elem->set_id(elemID);
        libmeshMesh.add_elem(elem);
    }
    libmeshMesh.set_spatial_dimension(V.cols());
    libmeshMesh.set_mesh_dimension(2);
    libmeshMesh.prepare_for_use();
}

libMesh::ElemType
getElemType(Eigen::MatrixXi& F)
{
    switch(F.cols())
    {
        case(3):
            return(libMesh::TRI3);
        
        case(4):
            return(libMesh::TET4);

        case(6):
            return(libMesh::TRI6);
        
        case(10):
            return(libMesh::TET10);
    }
    return libMesh::INVALID_ELEM;
}


bool
libmeshToCGAL(libMesh::Mesh& libmeshMesh, 
              CGAL::Polyhedron_3<CGAL::Exact_predicates_exact_constructions_kernel, CGAL::Polyhedron_items_with_id_3> & poly)
{
    // Definition of our cgalPoly, which will be the base for our
    // nefPoly
    // CGAL::Polyhedron_3 cgalPoly;

    // typedefs for cleanliness
    typedef typename CGAL::Polyhedron_3<CGAL::Exact_predicates_exact_constructions_kernel, CGAL::Polyhedron_items_with_id_3>::HalfedgeDS HalfedgeDS;
    typedef typename HalfedgeDS::Vertex Vertex;
    typedef typename Vertex::Point Point;
    CGAL::Polyhedron_incremental_builder_3<HalfedgeDS> B(poly.hds());
    B.begin_surface(libmeshMesh.n_nodes(), libmeshMesh.n_elem());

    // 
    for(int v = 0;v<libmeshMesh.n_nodes();v++)
    {
        libMesh::Node libNode = libmeshMesh.node_ref(v);
        B.add_vertex(Point(libNode(0), libNode(1), libNode(2)));
    }

    for(int f=0; f<libmeshMesh.n_elem(); f++)
    {
        libMesh::Elem* libPtr = libmeshMesh.elem_ptr(f);
        std::vector<libMesh::dof_id_type> connec;
        libPtr->connectivity(0, libMesh::IOPackage::VTK, connec);
        B.begin_facet();
        for(int c = 0;c<3;c++)
        {
            B.add_vertex_to_facet(connec[c]);
        }
        B.end_facet();
    }
    if(B.error())
    {
        B.rollback();
        return false;
    }
    B.end_surface();
    return poly.is_valid();
}

template<typename Polyhedron>
bool
CGALToLibmesh(libMesh::Mesh& libmeshMesh,
              CGAL::Polyhedron_3<CGAL::Exact_predicates_exact_constructions_kernel, CGAL::Polyhedron_items_with_id_3> & poly)
{
    return true;
}
