#include "libmeshToIgl.hpp"

void 
libMeshToIGL(libMesh::Mesh& libmeshMesh, Eigen::MatrixXd& V, Eigen::MatrixXi& F)
{
    V = Eigen::MatrixXd(libmeshMesh.n_nodes(), libmeshMesh.spatial_dimension());
    F = Eigen::MatrixXi(libmeshMesh.n_elem(), libmeshMesh.elem_ptr(0)->n_nodes());
    for(auto& node: libmeshMesh.local_node_ptr_range())
    {
        V(node->id(), 0) = (*node)(0);
        V(node->id(), 1) = (*node)(1);
        
        if(libmeshMesh.spatial_dimension() == 3)
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