#include "surfaceMeshing.hpp"
#include <algorithm>

void isElementSurface(libMesh::Elem& element, std::vector<int>& elSet, 
                      std::vector<int>& surfaceFaces)
{
    int neighbor_counter = 0;
    for(int side = 0; side < element.n_sides(); side++)
    {   
        auto neighbor = element.neighbor_ptr(side);
        if(neighbor == nullptr)
        {
            surfaceFaces[neighbor_counter++] = side;
        }
        else
        {
            if(!std::binary_search(elSet.begin(), elSet.end(), neighbor->id()))
            {
                surfaceFaces[neighbor_counter++] = side;
            }
        }
    }
}


void getElemInfo(libMesh::ElemType& elem_type, libMesh::ElemType& face_type, 
                 libMesh::Elem* element, int& num_elem_faces, int& num_face_nodes)
{
    elem_type = element->type();
    
    switch (elem_type)
    {
        case (libMesh::HEX8):
            num_elem_faces = 6;
            num_face_nodes = 4;
            face_type = libMesh::QUAD4;
            break;
            
        case (libMesh::HEX27):
            num_elem_faces = 6;
            num_face_nodes = 9;
            break;
        
        case (libMesh::TET4):
            num_elem_faces = 4;
            num_face_nodes = 3;
            face_type = libMesh::TRI3;
            break;

        case (libMesh::TET10):
            num_elem_faces = 4;
            num_face_nodes = 6;
            break;
    }
}


void getSurface(libMesh::Mesh& mesh, libMesh::Mesh& surfaceMesh, std::vector<int>& elSet)
{   
    //LibMesh method that has to be run in order to access neighbor info
    mesh.find_neighbors();

    //Variables to store element information so it is easily accessible later
    //This implementation does assume only one element type is used
    libMesh::ElemType elem_type, face_type; 
    int num_elem_faces, num_face_nodes;

    // //Counter to store the number of surface elements
    int surface_elem_counter = 0;

    //Map from old node ids to the new ones in the surface mesh
    std::vector<int> currentNodeIds;
    std::map<int, int> newNodeIds;

    //Connectivity of all the elements in the surface mesh
    std::vector<int> connectivity;

    //Use getElemInfo method to retrieve Element Info 
    libMesh::Elem* elem = mesh.elem_ptr(0);
    getElemInfo(elem_type, face_type, 
                elem, num_elem_faces, num_face_nodes);
    
    // Loops over all the elements in the input vector 
    for(int elemNum: elSet)
    {
        //Get ptr to current element
        libMesh::Elem& element = mesh.elem_ref(elemNum);

        //Initialise vecotr to store sides of element that are on surface
        //, initialise all elements as -1, as this will be used to indicate
        //  there are no more surface elements
        std::vector<int> surfaceFaces(num_elem_faces, -1);

        //Method to check whether the current element has faces that are on the surface
        //Stores these faces (if they exist) in surfaceFaces vector
        isElementSurface(element, elSet, surfaceFaces);
        
        for(int i = 0; surfaceFaces[i] != -1 && i<element.n_sides(); i++)
        {
            std::vector<unsigned int> nodes_on_side = element.nodes_on_side(surfaceFaces[i]);

            for(auto localNodeId: nodes_on_side)
            {
                int globalNodeId = element.node_id(localNodeId);
                connectivity.push_back(globalNodeId);
                currentNodeIds.push_back(globalNodeId);
            }
            surface_elem_counter++;
        }
        
    }

    //Sorts the node ids in the currentNodeIds in numerical order and removes duplicates
    std::sort(currentNodeIds.begin(), currentNodeIds.end());
    std::vector<int>::iterator newEnd;
    newEnd = std::unique(currentNodeIds.begin(), currentNodeIds.end());
    currentNodeIds.resize(std::distance(currentNodeIds.begin(), newEnd));

    //Map from the currentNodeIds to the new node ids in the surface mesh
    for(int i = 0; i < (int) currentNodeIds.size(); i++)
    {
        newNodeIds[currentNodeIds[i]] = i;
    }
    
    //Using the newNodeIds map, set all the node data needed for the new mesh 
    for(auto nodeId: currentNodeIds)
    {   
        libMesh::Node* node = mesh.node_ptr(nodeId);
        double pnt[3];
        pnt[0] = (*node)(0);
        pnt[1] = (*node)(1);
        pnt[2] = (*node)(2);
        libMesh::Point xyz(pnt[0], pnt[1], pnt [2]);
        surfaceMesh.add_point(xyz, newNodeIds[nodeId]);
    }

    //For all of the surface elements, create the representitive 2D libmesh element 
    //Connectivity is set and the element is added to the new mesh
    for(int i = 0; i < surface_elem_counter; i++)
    {
        libMesh::Elem* elem = libMesh::Elem::build(face_type).release();
        for(int j = 0; j < num_face_nodes; j++)
        {
            elem->set_node(j) = surfaceMesh.node_ptr(newNodeIds[connectivity[(i*num_face_nodes)+j]]);
        }
        elem->set_id(i);
        surfaceMesh.add_elem(elem);
    }

    //Set mesh dimensions 
    surfaceMesh.set_mesh_dimension(2); //Should this be 2 or 3???
    surfaceMesh.set_spatial_dimension(3);
}