#include "surfaceMeshing.hpp"
#include <algorithm>
// isElementSurface for if user wants the whole mesh skinned
void isElementSurface(libMesh::Elem& element, std::vector<int>& surfaceFaces)
{
    int neighbor_counter = 0;
    for(int side = 0; side < element.n_sides(); side++)
    {   
        // std::cout << side << std::endl;
        auto neighbor = element.neighbor_ptr(side);
        if(neighbor == nullptr)
        {
            surfaceFaces[neighbor_counter++] = side;
        }
    }
}


// isElementSurface for if user specifies an elSet in getSurface
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

// Get surface for when the user just wants the whole mesh skinned, and not a subsection of it
//  i.e. they haven't specified an elSet
void getSurface(libMesh::Mesh& mesh, libMesh::Mesh& surfaceMesh)
{   
    std::cout << "Beginning skinning mesh" << std::endl;
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
    for(int elem = 0; elem< mesh.n_elem(); elem++)
    {
        //Get ptr to current element
        libMesh::Elem& element = mesh.elem_ref(elem);

        //Initialise vecotr to store sides of element that are on surface
        //, initialise all elements as -1, as this will be used to indicate
        //  there are no more surface elements
        std::vector<int> surfaceFaces(num_elem_faces, -1);
        // surfaceFaces.reserve(num_elem_faces);

        //Method to check whether the current element has faces that are on the surface
        //Stores these faces (if they exist) in surfaceFaces vector
        isElementSurface(element, surfaceFaces);
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
    surfaceMesh.prepare_for_use();

    std::cout << "Created skinned mesh" << std::endl;
}


// Get surface for when the user DOES want only a subset of the mesh skinned
void getSurface(libMesh::Mesh& mesh, libMesh::Mesh& surfaceMesh, std::vector<int>& elSet)
{   
    std::cout << "Beginning skinning mesh" << std::endl;
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
        // surfaceFaces.reserve(num_elem_faces);

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
    surfaceMesh.prepare_for_use();

    std::cout << "Created skinned mesh" << std::endl;
}


void groupElems(libMesh::Mesh mesh, std::vector<std::vector<libMesh::dof_id_type>>& groups)
{
    std::set<int> elems;
    for(auto elem : mesh.element_ptr_range())
    {
        elems.insert(elem->id());
    }

    while(!elems.empty())
    {
        auto it = elems.begin();
        libMesh::dof_id_type next = *(it);
        elems.erase(it);
        std::set<libMesh::dof_id_type> neighbors;
        //Adding first element to nesighbors
        neighbors.insert(next);
        std::vector<libMesh::dof_id_type> groupElems(0);

        //While elements exist in neighbors
        while(!neighbors.empty()){

            std::set<libMesh::dof_id_type> new_neighbors;

            for(auto& next : neighbors){
                //Add current elems to groupElems vector
                groupElems.push_back(next);
                
                // Get the libMesh element
                libMesh::Elem& elem = mesh.elem_ref(next);

                // How many nearest neighbors (general element)?
                unsigned int NN = elem.n_neighbors();

                // Loop over neighbors
                for(unsigned int i=0; i<NN; i++){

                    const libMesh::Elem * nnptr = elem.neighbor_ptr(i);
                    // If on boundary, some may be null ptrs
                    if(nnptr == nullptr){
                        std::cout << "null\n";
                        continue;
                    }
                    libMesh::dof_id_type idnn = nnptr->id();

                    // Select only those that exist within elems set
                    if(elems.find(idnn)!= elems.end()){
                        //Add these neighbor ids to new_neighbors
                        new_neighbors.insert(idnn);
                        // Remove these neighbor ids from elems
                        elems.erase(idnn);
                    }
                }

            }
            // End loop over previous neighbors
            // Found all the new neighbors, done with current set.
            neighbors = new_neighbors;
        }
        
        groups.push_back(groupElems);

    }
}

void saveGroupedElems(libMesh::LibMeshInit& init, libMesh::Mesh& surfaceMesh, 
                      std::vector<std::vector<libMesh::dof_id_type>>& groups,
                      std::string componentFilename){
    unsigned int count = 0;

    if(groups.size() == 1){
        std::cout << "All elements belong to one entity already, no grouping necessary" << std::endl;
        return;
    }

    for(auto& vector : groups){
        //Mesh of subdomain we are going to save
        libMesh::Mesh newMesh(init.comm());
        libMesh::ElemType face_type;
        unsigned int num_elems, num_face_nodes;
        newMesh.clear();
        face_type = (surfaceMesh.elem_ref(0)).type();
        num_elems = vector.size();
        std::vector<unsigned int> current_nodes_ids;
        std::vector<unsigned int> connectivity;
        num_face_nodes = (surfaceMesh.elem_ref(0)).n_nodes();
        current_nodes_ids.reserve(num_elems*num_face_nodes);
        connectivity.reserve(num_elems*num_face_nodes);

        for(auto& elementID : vector){
            libMesh::Elem& elem = surfaceMesh.elem_ref(elementID);
            for(auto& node: elem.node_ref_range()){
                current_nodes_ids.emplace_back(node.id());
                connectivity.emplace_back(node.id());
            }
        }

        std::sort(current_nodes_ids.begin(), current_nodes_ids.end());
        std::vector<unsigned int>::iterator newEnd;
        newEnd = std::unique(current_nodes_ids.begin(), current_nodes_ids.end());
        current_nodes_ids.resize(std::distance(current_nodes_ids.begin(), newEnd));
        std::map<unsigned int, unsigned int> nodeMap;

        for(int i = 0; i < current_nodes_ids.size(); i++){
            nodeMap[current_nodes_ids[i]] = i;
        }

        for(auto& nodeID: current_nodes_ids)
        {   
            libMesh::Node* node = surfaceMesh.node_ptr(nodeID);
            double pnt[3];
            pnt[0] = (*node)(0);
            pnt[1] = (*node)(1);
            pnt[2] = (*node)(2);
            libMesh::Point xyz(pnt[0], pnt[1], pnt [2]);
            newMesh.add_point(xyz, nodeMap[nodeID]);
        }

        //For all of the surface elements, create the representitive 2D libmesh element 
        //Connectivity is set and the element is added to the new mesh
        for(int i = 0; i < num_elems; i++)
        {
            // Create elem to add
            libMesh::Elem* elem = libMesh::Elem::build(face_type).release();

            for(int j = 0; j < num_face_nodes; j++)
            {
                elem->set_node(j) = newMesh.node_ptr(nodeMap[connectivity[(i*num_face_nodes)+j]]);
            }
            elem->set_id(i);
            newMesh.add_elem(elem);
        }
        newMesh.set_mesh_dimension(2); //Should this be 2 or 3???
        newMesh.set_spatial_dimension(3);
        newMesh.prepare_for_use();
        std::string newMeshName = componentFilename + std::to_string(count) + ".e";
        newMesh.write(newMeshName);
        
        count++;
    }
}