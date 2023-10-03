#include "libmesh_test_init.hpp"

std::string exe_name = "tests";
std::vector<char *> libmesh_argv = {(char *)exe_name.data()};
libMesh::LibMeshInit init =
    libMesh::LibMeshInit(libmesh_argv.size() - 1, libmesh_argv.data());
