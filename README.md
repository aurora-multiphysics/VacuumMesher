# libmeshSkinning
The tools in this repo utilise LibMesh and libIGL to help produce a "vacuum mesh" (implicit complement) from the mesh
of an initial part geometry. The repo is misleadingly called libmesh skinning when it should be called "Vacuum mesh generation" or something of that ilk, but hopefully this read me helps clarify the functionality implemented here.

# Available functionality
## Mesh Skinning
One of the tools available is a mesh skinner. This will take an input mesh, and return to you the skin of that mesh. This can either be the skin of the whole mesh, or the skin of a subset of the elements of the mesh. If the user asks for the skin of set of elements in the mesh, and these elements are discontinous, the user may prompt these various different parts to be output as seperate files. Discontinous in this case refers to disconintous at the sides of elements. If two elements are only touching at an edge, they are considered discontinous. In future this functionality can be improved.   

## Vacuum region generation 
The "vacuum region" surrounding a piece of geometry (i.e. a coil) is often not modolled when the part is initially meshed. Functionality in this repo allows for the vacuum region of an initial part to be generated. This vacuum mesh will conform to the nodes/ facets on the boundary of the original part's mesh. This new mesh will then be recombined with the mesh of the original part, ready to be used in problems that require a vacuum region to be modelled.

## Duplicate Node Removal
As part of the vacuum region generation, the vacuum mesh must be recombined with the original mesh. As both meshes will share facets at the boundary of the original part, there will be duplicate facets/nodes present. This is never good news. Using an rTree, each node is checked to see if it is a duplicate of a pre-existing node. If a duplicate is found, it is not added to the final mesh, and the element connectivity of all the elements is changed to account for the change in node id's that occurs due to not adding these duplicate nodes.

# Building
To build everything, follow a very familiar cmake build procedure with a couple of extra arguments. Also important to mention is that this tool has been made to accomodate for a pre-existing moose install, as I imagine most of the people using it will have one, and therefore probably have an existing libmesh install that can be used. I have only tested this will a "from source" MOOSE build. If you used conda to install MOOSE, seek help and/or a hug. 

- Download libIGL from "url here". LibIGL is used for tetrahedralisation of the vacuum region.

- First create a build folder and `cd` into it

`
mkdir build
cd build
`
- Next run `cmake ..`, with two extra arguments. These arguments are
```
-DFETCHCONTENT_SOURCE_DIR_LIBIGL=<path-to-libigl>
```
and
```
-DLIBMESH_CONF_DIR
```
- The first argument specifies where the downloaded libIGL source is, the second where to locate the libmesh-configure exextutable is, which will allow cmake to easily fetch the necessary includes and libs to get libmesh working

# Examples

### gotcha's
Currently all the functionality of all the tools in this repo only apply to tet/tri meshes, as this is what libIGL primarily supports. But if this changes at some ambiguous, arbitrary time in the future, then these tools can be updated to provide functionality for other elements types. 


### To do's (More for me than anyone else)
-Make sure the normals on the geometry/vacuum boundary point OUTWARD into the vacuum for MOOSE reasons
-Get libmesh to CGAL working 

