# VacuumMesher
The tools in this repo utilise LibMesh and libIGL to generate a "vacuum mesh" from the mesh
of an initial part geometry. Vacuum meshes generated using this tool will conform to the input mesh.

# Layout
This repo is sort of split into two parts. Firstly, there is the VacuumMesher library itself, the constituents of which can be found within the fittingly named "VacuumMesher" directory. This directory contains all of the necessary source and header files to build the VacuumMesher library, which may then be linked into any applications you wish to use it in. Also within this directory is a git submodule link to libIGL, a dependency of VacuumMesher. 

Secondly, there are the examples and functionality found in the root directory. These examples should provide the functionality that most users will need. The source files for these can be found in the "./bins/" directory. Within the source and include directories in the root dir, there exists some additional functionality which is used within the examples. This functionality consists of the  MeshContainer and MeshPair classes. 

The MeshPair class stores both a libmesh and libIGL representations of a mesh in one place. Because both libmesh and libIGL functionality is used within VacuumMesher, often both representations are needed. It would be possible to have only one representation present and to just generate the other format within the scope of functions that need it. However this would be inefficient if the both representations are needed multiple times.  

The MeshContainer class has ownership of multiple MeshPair objects, for the frequently needed meshes. For example you will start off with the mesh you input, the "UserMesh". Then once it is skinned you have the "SkinnedMesh". It is necessary to generate a boundary around the SkinnedMesh to describe the vacuum region. The mesh describing this boundary is called the "BoundaryMesh", and then finally once the vacuum region is generated you have the "VacuumMesh". All in all, there are quite a few meshes needed, so having a class to keep track of them is handy. 

# Available functionality
## Mesh Skinning
The first piece of functionality . This will take an input mesh, and return to you the skin of that mesh. This can either be the skin of the whole mesh, or the skin of a subset of the elements of the mesh. If the user asks for the skin of set of elements in the mesh, and these elements are discontinous, the user may prompt these various different parts to be output as seperate files. Discontinous in this case refers to disconintous at the sides of elements. If two elements are only touching at an edge, they are considered discontinous. In future this functionality can be improved.   

## Vacuum region generation 

### Normal
The "vacuum region" surrounding a piece of geometry (i.e. a coil) is often not modolled when the part is initially meshed. Functionality in this repo allows for the vacuum region of an initial part to be generated. This vacuum mesh will conform to the nodes/ facets on the boundary of the original part's mesh. This new mesh will then be recombined with the mesh of the original part, ready to be used in problems that require a vacuum region to be modelled.

### Coils 


## Duplicate Node Removal
As part of the vacuum region generation, the vacuum mesh must be recombined with the original mesh. As both meshes will share facets at the boundary of the original part, there will be duplicate facets/nodes present. This is never good news. Using an rTree, each node is checked to see if it is a duplicate of a pre-existing node. If a duplicate is found, it is not added to the final mesh, and the element connectivity of all the elements is changed to account for the change in node id's that occurs due to not adding these duplicate nodes.

# Building
To build the example binaries and the lib, a familiar cmake build procedure with a couple of extra arguments should be used. Also important to mention is that this tool has been made to accomodate for a pre-existing moose install, as I imagine most of the people using it will have one and hence probably have an existing libmesh install that can be used. I have only tested this will a "from source" MOOSE build. If you used conda to install MOOSE, seek help and/or a hug. 

- First create a build folder and `cd` into it

`
mkdir build
cd build

`
- Next run `cmake ..`, with one extra argument.

```
-DLIBMESH_DIR=/path/to/moose/libmesh/installed
```
This argument specifies to VacuumMesher where to look for libmesh libs and includes.

# Examples

