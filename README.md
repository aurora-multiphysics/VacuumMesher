A small tool that offers the functionality to get the "skin" of a mesh using libMesh. Eventually
this will also offer the functionality to create the mesh of the vacuum space surrounding a piece of
geometry. To build use the standard build process detailed below

```
mkdir build
cd build
cmake ../
make

```

The appropriate mesh files for the example problems can be found on the [Digital Tools sharepoint]().
To use this tool for your own problems, use the example problems as a template. In the `std::set<int> Elems` seen
in all the examples, put the element ID's you would like to obtain the skin of. These can be discontinuous, but currently all
elements will be saved into the same mesh. 