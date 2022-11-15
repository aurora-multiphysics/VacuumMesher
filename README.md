# libmeshSkinning

## What is this?

The tools in this repo utilise LibMesh and libIGL to perform various functions on an input mesh.
Initially this was all centred around obtaining the surface of a mesh, or the 'skin', but has developed and flourished to offer other functionality as well.

## Mesh skinning

One of the tools available is a mesh skinner. This will take an input mesh, and return to you the skin of that mesh. This can either be the skin of the whole mesh, or the skin of a subset of the elements of the mesh. If the user asks for the skin of set of elements in the mesh, and these elements are discontinous, the user may prompt these various different parts to be output as seperate files. Discontinous in this case refers to disconintous at the sides of elements. If two elements are only touching at an edge, they are considered discontinous. In future this functionality can be improved.   

## Vacuum region generation 
Leading on from the mesh skinning is the ability to generate a tetragedra

## Duplicate Node Removal


### gotcha's
Currently all the functionality of all the tools in this repo only apply to tet/tri meshes, as this is what libIGL primarily supports. But if this changes at some ambiguous, arbitrary time in the future, then these tools can be updated to provide functionality for other elements types. 


### To do's (More for me than anyone else)
-Make sure the normals on the geometry/vacuum boundary point OUTWARD into the vacuum for MOOSE reasons
-Get libmesh to CGAL working 

