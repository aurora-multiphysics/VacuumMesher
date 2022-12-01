#include "generateBoundaries.hpp"

void createBoundary(libMesh::LibMeshInit &init, libMesh::Mesh& surfMesh)
{
    auto box = libMesh::MeshTools::create_bounding_box(surfMesh);
    libMesh::Mesh cubeMesh(init.comm());
    libMesh::Mesh boundaryMesh(init.comm());
    libMesh::Point centre;

    // Scale the bounding box by a scaling factor
    double max_length = 0;
    box.max().print();
    box.min().print();

    for(int i = 0; i<3; i++)
    {
        centre(i) = box.max()(i) - ((box.max()(i) - box.min()(i))/2);
        max_length = std::abs(box.max()(i) - box.min()(i)) > max_length ? std::abs(box.max()(i) - box.min()(i)) : max_length;
    }
    centre.print();
    std::cout << max_length << std::endl;
    for(int i = 0; i<3;i++)
    {
        box.min()(i) = centre(i) - max_length;
        box.max()(i) = centre(i) + max_length;
    }

    box.min() *= 1.5;
    box.max() *= 1.5;

    // Generate a cube which we will then get the skin of to create boundary mesh
    libMesh::MeshTools::Generation::build_cube(cubeMesh, 5, 5, 5, 
                                              (box.min())(0), (box.max())(0), 
                                              (box.min())(1), (box.max())(1), 
                                              (box.min())(2), (box.max())(2), 
                                              libMesh::TET4);
    // Skin the cubeMesh
    getSurface(cubeMesh, boundaryMesh, true, "boundarySkinTest.e");

    // map to remember node ids
    std::map<unsigned int, unsigned int> id_map;
    // Merge meshes
    unsigned int node_id = 1 + surfMesh.max_node_id();
    for(auto& node: boundaryMesh.local_node_ptr_range())
    {
        
        id_map[(node->id())] = node_id;
        double pnt[3];
        // Set pnt[i] to equal the coordinates of node(i) 
        // for(int i = 0; i<3; i++){pnt[i] = (*node)(i);}boundaryMesh.node_ptr(elem->node_ptr(j)->
        pnt[0] = (*node)(0);
        pnt[1] = (*node)(1);
        pnt[2] = (*node)(2);

        libMesh::Point xyz(pnt[0], pnt[1], pnt[2]);
        surfMesh.add_point(xyz, node_id);
        node_id++;
    }

    unsigned int elem_id = 1 + surfMesh.max_elem_id();
    for(auto& elem: boundaryMesh.element_ptr_range())
    {
        libMesh::Elem* newElem = libMesh::Elem::build(elem->type()).release();
        for(int j = 0; j < newElem->n_nodes(); j++)
        {
            newElem->set_node(j) = surfMesh.node_ptr(id_map[elem->node_ptr(j)->id()]);
        }
        newElem->set_id(elem_id);
        surfMesh.add_elem(newElem);
        elem_id++;
    }
    surfMesh.prepare_for_use();
}