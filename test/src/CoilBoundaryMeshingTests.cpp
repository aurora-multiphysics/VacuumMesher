#include "BoundaryGeneration/CoilBoundaryGenerator.hpp"
#include "catch2/catch_all.hpp"
#include "catch2/catch_test_macros.hpp"
#include "libmesh_test_init.hpp"

TEST_CASE("CoilBoundGenerator", "[surface]") {

  libMesh::Mesh mesh(init.comm());
  libMesh::Mesh surface_mesh(init.comm());
  libMesh::Mesh boundary_mesh(init.comm());

  SECTION("changeMeshBasis") {

    mesh.read(std::string(USER_DIR) +
              "/test/testingMeshes/unit_cube/unit_cube.e");
    CoilBoundaryGenerator coil_bound_generator(mesh, surface_mesh,
                                               boundary_mesh);

    // Make simple matrix to change base of
    Eigen::MatrixXd vertices(3, 3);
    vertices << 1, 0, 0, 0, 1, 0, 0, 0, 1;

    Eigen::Matrix3d new_verts;
    new_verts << 0, 2, 0, 0, 0, 2, 2, 0, 0;

    Eigen::Matrix3d new_basis;
    new_basis << 0, 0.5, 0, 0, 0, 0.5, 0.5, 0, 0;

    Eigen::Vector3d new_origin = {0, 0, 0};

    coil_bound_generator.changeMeshBasis(vertices, new_origin, new_basis);
    REQUIRE(new_verts == vertices);
  }

  SECTION("coplanarSeedPoints") {

    mesh.read(std::string(USER_DIR) +
              "/test/testingMeshes/sideset_cube/sideset_cube.e");
    CoilBoundaryGenerator coil_bound_generator(mesh, surface_mesh,
                                               boundary_mesh);

    Eigen::MatrixXd seed_points(2, 3);

    coil_bound_generator.getCoplanarSeedPoints(mesh, seed_points);

    REQUIRE(-1 < seed_points(0, 0));
    REQUIRE(seed_points(0, 0) < 1);

    REQUIRE(1 < seed_points(0, 2));
    REQUIRE(seed_points(0, 2) < 3);

    REQUIRE(-1 < seed_points(1, 0));
    REQUIRE(seed_points(1, 0) < 1);

    REQUIRE(-3 < seed_points(1, 2));
    REQUIRE(seed_points(1, 2) < 1);
  }
}