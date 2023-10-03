#include "BoundaryGeneration/BoundaryGenerator.hpp"
#include "catch2/catch_all.hpp"
#include "catch2/catch_test_macros.hpp"
#include "libmesh_test_init.hpp"

TEST_CASE("BoundaryGenerator", "[cube]") {

  // Libmesh mesh setup
  libMesh::Mesh mesh(init.comm());
  libMesh::Mesh surface_mesh(init.comm());
  libMesh::Mesh boundary_mesh(init.comm());

  std::string tri_flags = "qYa";
  mesh.read(std::string(USER_DIR) +
            "/test/testingMeshes/unit_cube/unit_cube.e");

  SECTION("genSquare") {

    // Boundary Parameters
    double length = 10;
    int subdivisions = 10;

    //
    BoundaryGenerator bound_generator(mesh, surface_mesh, boundary_mesh);
    Eigen::MatrixXd square_vertices;
    Eigen::MatrixXi square_elems;

    // Generate square made of edge elements
    bound_generator.genSquare(square_vertices, square_elems, length,
                              subdivisions);

    REQUIRE(std::abs(square_vertices.row(0).minCoeff()) == (length / 2));
    REQUIRE(std::abs(square_vertices.row(0).maxCoeff()) == (length / 2));
    REQUIRE(square_vertices.rows() == (subdivisions * 4));
  }

  SECTION("getSeedingPoints") {

    // Boundary Parameters
    // Boundary should be too small resulting in exception being thrown
    double length = 0.1;
    int subdivisions = 10;

    //
    BoundaryGenerator bound_generator(mesh, surface_mesh, boundary_mesh);
    Eigen::MatrixXd boundary_vertices;
    Eigen::MatrixXi boundary_elems;

    // Generate square made of edge elements
    REQUIRE_THROWS_AS(bound_generator.checkBoundary(length),
                      std::invalid_argument);
  }

  SECTION("throw") {}
}