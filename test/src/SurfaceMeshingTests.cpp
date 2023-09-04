#include "catch2/catch_all.hpp"
#include "catch2/catch_test_macros.hpp"
#include "SurfaceMeshing/surfaceMeshing.hpp"
#include "MeshContainer.hpp"

std::string appName = "tests";
std::vector<char *> libmeshArgv = {(char *)appName.data()};
libMesh::LibMeshInit init(libmeshArgv.size() - 1, libmeshArgv.data());




TEST_CASE( "Factorials are computed", "[factorial]" ) {
    REQUIRE( 1 == 1 );
}