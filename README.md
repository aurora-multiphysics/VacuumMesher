# VacuumMesher
The tools in this repo utilise LibMesh and libIGL to generate a "vacuum mesh" from the mesh
of an initial part geometry. Vacuum meshes generated using this tool will conform correctly to the input mesh.

# Layout
There are two parts to this repo. Firstly, there is the VacuumMesher library itself, the constituents of which can be found within the fittingly named "VacuumMesher" directory. This directory contains all of the necessary source and header files to build the VacuumMesher library, which may then be linked into any applications you wish to use it in. Also within this directory is a git submodule link to libIGL, a dependency of VacuumMesher. Secondly, there are the examples and functionality found in the root directory. These examples should provide the functionality that most users will need. The source files for these can be found in the "./bins/" directory. Within the source and include directories in the root dir, there exists some additional functionality which is used within the examples. This functionality consists of the  MeshContainer and MeshPair classes. 

# Building
To build the example binaries and the lib, a familiar cmake build procedure with a couple of extra arguments should be used. Also important to mention is that this tool has been made to accomodate for a pre-existing moose install, as I imagine most of the people using it will have one and hence probably have an existing libmesh install that can be used. I have only tested this will a "from source" MOOSE build. If you used conda to install MOOSE, seek help and/or a hug. 

- Firstly clone the repo.
``` git clone https://github.com/aurora-multiphysics/VacuumMesher.git```

- Secondly, enter the repo and update the git submodules to get Catch2 and libIGL
``` 
cd VacuumMesher
git submodule update --init --recursive
```

- Now create a build folder and `cd` into it

```
mkdir build
cd build
```
- Next run cmake, with an extra argument `-DLIBMESH_DIR`.

```
cmake -DLIBMESH_DIR=/path/to/moose/libmesh/installed ..
```
This argument specifies to VacuumMesher where to look for libmesh libs and includes.

# Included examples

6 examples are included, the code for which can be found in `./bin/`. These examples shoud provide the functionality needed by most users.

## skin
Skins the input mesh, maintaining any existing sidesets on the boundary.

An example run from the command line would look something like:
`./build/bin/skin -i Examples/example_cube.e -o example_skinned.e`

## boundary
Generates the skinned mesh with a cubic boundary surrounding it, but does not generate any tetrahedra / the vacuum region. 

An example run from the command line would look something like:
`./build/bin/boundary -i Examples/example_cube.e -o example_boundary.e --bound_len 2`

## coilBoundary
Same as `boundary`, but for a coil problem. Specifically a coil problem where the coil input/ output sidesets need to be coplanar with the vacuum boundary.

An example run from the command line would look something like:
`./build/bin/coilBoundary -i ./Examples/example_coil.e -o example_coil_boundary.e --bound_len 100 --sideset_one_id 1 --sideset_two_id 2`

## vacuum
Generates the vacuum mesh around the input mesh. Firstly the mesh is skinned, and then a cubic boundary is generated around the skinned mesh.
Then tetrahedra are generated in the defined region between the skinned mesh and the boundary. Finally the generated vacuum region is combined
with the original input mesh.

An example run from the command line would look something like:
`./build/bin/vacuum -i ./Examples/example_mesh.e -o example_vac.e --bound_len 2`

## coilVacuum 
Provides the same functionality as vacuum but for a coil problem.

An example run from the command line would look something like:
`./build/bin/coilVacuum -i ./Examples/example_coil.e -o example_coil_vac.e --bound_len 100 --sideset_one_id 1 --sideset_two_id 2`

# Flags
There are several flags you can pass to the example binaries. These influence the generation of the boundary

| First Header  | Second Header |
| ------------- | ------------- |
| `-h`, `--help`  | Prints a help message showing all of the options detailed here  |
| `-v`, `--verbose`  | Passes verbose flag onto tetgen/ triangle, hence printing out more information about element generation. |
| `-i`, `--input`  | The input mesh used for mesh generation. |
| `-o`, `--output`  | Optional output name for the generated mesh. Remember to include the file extension!. If not given a default name is given|
| `--max_tet`  | Enforces a maximum tetrahedra volume on all the tets generated. |
| `--max_tri`  | Prints a help message showing all of the options detailed here. |
| `--bound_subdiv` | Defines the number of subdivisions along one edge of the cubic boundary. |
| `--bound_len` | Defines the edge length of the cubic boundary of the vacuum region |
| `--sideset_one_id` | For a coil problem, this option defines the ID of your first coil sideset that you want to be coplanar with the boundary. |
| `--sideset_two_id` | For a coil problem, this option defines the ID of your second coil sideset that you want to be coplanar with the boundary. |




