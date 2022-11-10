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
    libmeshMesh.clear();
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
    }

    {
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
    }
    // libmeshMesh.set_spatial_dimension(V.cols());
    // libmeshMesh.set_mesh_dimension(2);
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

template<typename Polyhedron>
void
libmeshToCGAL(libMesh::Mesh& libmeshMesh, 
              Polyhedron& poly)
{
    // typedefs for cleanliness
    typedef typename Polyhedron::HalfedgeDS HalfedgeDS;
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

    B.end_surface();
    return;
}

template<typename Polyhedron>
void
CGALToLibmesh(libMesh::Mesh& libmeshMesh,
              Polyhedron & poly)
{
    // typdef the vertex iterator for cleanliness
    typedef typename Polyhedron::Vertex_const_iterator Vertex_iterator;
    // Clear libmesh mesh incase anything is in it
    libmeshMesh.clear();
    // 
    std::map<Vertex_iterator, unsigned int> vertex_to_index;
    // Counter to give nodes correct ID in libmesh mesh
    unsigned int id = 0;
    {
        double pnt[3];
        double x,y,z;
        // Loop over vertices in cgal mesh to 
        for(Vertex_iterator p = poly.vertices_begin(); 
            p != poly.vertices_end();
            p++)
        {
            pnt[0]=CGAL::to_double(p->point().x());
            pnt[1]=CGAL::to_double(p->point().y());
            pnt[2]=CGAL::to_double(p->point().z());
            libMesh::Point xyz(pnt[0], pnt[1], pnt[2]);
            libmeshMesh.add_point(xyz, id);
            vertex_to_index[p] = id;
            id++;
        }
    }
    
    {
        for(typename Polyhedron::Facet_const_iterator facet = poly.facets_begin();
            facet != poly.facets_end();
            ++facet)

        {
            // Create libmesh element that represents the CGAL facet, and release it from it's unique_ptr
            libMesh::Elem* elem = libMesh::Elem::build(libMesh::TRI3).release();

            typename Polyhedron::Halfedge_around_facet_const_circulator he = facet->facet_begin();
            unsigned int j = 0;
            do 
            {
                elem->set_node(j) = libmeshMesh.node_ptr(vertex_to_index[he->vertex()]);
                j++;
            } while ( ++he != facet->facet_begin());
            libmeshMesh.add_elem(elem);
        }
    }
    libmeshMesh.prepare_for_use();
    
}

template void libmeshToCGAL(libMesh::Mesh& libmeshMesh, CGAL::Polyhedron_3<CGAL::Exact_predicates_exact_constructions_kernel, CGAL::Polyhedron_items_with_id_3> & poly);

template void CGALToLibmesh(libMesh::Mesh& libmeshMesh, CGAL::Polyhedron_3<CGAL::Exact_predicates_exact_constructions_kernel, CGAL::Polyhedron_items_with_id_3> & poly);


