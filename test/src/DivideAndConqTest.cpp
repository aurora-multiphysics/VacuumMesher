#include "Utils/divConq.hpp"
#include "catch2/catch_all.hpp"
#include "catch2/catch_test_macros.hpp"
#include "libmesh_test_init.hpp"
#include <catch2/matchers/catch_matchers_floating_point.hpp>

TEST_CASE("DivideAndConquer", "[Pyramid]") {

  // Libmesh mesh setup
  libMesh::Mesh mesh(init.comm());
  libMesh::Mesh surface_mesh(init.comm());
  libMesh::Mesh boundary_mesh(init.comm());

  std::string tri_flags = "qYa";

  SECTION("CheckDistance3D") {

    mesh.read(std::string(USER_DIR) +
              "/test/testingMeshes/DivConqMesh/DivConqTetMesh.e");

    VacuumMesher::ClosestPairFinder divConq(mesh);
    double closestDistance = divConq.closestPairMagnitude(3);

    REQUIRE(closestDistance == Catch::Approx(1.1693527));
  }

  SECTION("CheckDistance2D") {

    mesh.read(std::string(USER_DIR) +
              "/test/testingMeshes/DivConqMesh/DivConqTriMesh.e");

    VacuumMesher::ClosestPairFinder divConq(mesh);
    double closestDistance = divConq.closestPairMagnitude(2);

    REQUIRE(closestDistance == Catch::Approx(1));
  }
}