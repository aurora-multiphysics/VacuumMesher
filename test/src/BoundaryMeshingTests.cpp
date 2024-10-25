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

  SECTION("checkBoundary") {

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

  SECTION("genRect") {

    // Boundary Parameters
    // Boundary should be too small resulting in exception being thrown
    double x_dim = 1;
    double y_dim = 1.5;
    int x_subdiv = 2;
    int y_subdiv = 3;


    //
    BoundaryGenerator bound_generator(mesh, surface_mesh, boundary_mesh);
    Eigen::MatrixXd boundary_verts;
    Eigen::MatrixXi boundary_elems;

    Eigen::MatrixXd correct_boundary_verts(10, 2);
    correct_boundary_verts(0, 0) = -0.5;
    correct_boundary_verts(0, 1) = -0.75;

    correct_boundary_verts(1, 0) = 0.0;
    correct_boundary_verts(1, 1) = -0.75;

    correct_boundary_verts(2, 0) = 0.5;
    correct_boundary_verts(2, 1) = -0.75;

    correct_boundary_verts(3, 0) = 0.5;
    correct_boundary_verts(3, 1) = -0.25;

    correct_boundary_verts(4, 0) = 0.5;
    correct_boundary_verts(4, 1) = 0.25;

    correct_boundary_verts(5, 0) = 0.5;
    correct_boundary_verts(5, 1) = 0.75;

    correct_boundary_verts(6, 0) = 0.0;
    correct_boundary_verts(6, 1) = 0.75;

    correct_boundary_verts(7, 0) = -0.5;
    correct_boundary_verts(7, 1) = 0.75;

    correct_boundary_verts(8, 0) = -0.5;
    correct_boundary_verts(8, 1) = 0.25;
    
    correct_boundary_verts(9, 0) = -0.5;
    correct_boundary_verts(9, 1) = -0.25;

   
    Eigen::MatrixXi correct_boundary_elems(10, 2);
    correct_boundary_elems(0, 0) = 0;
    correct_boundary_elems(0, 1) = 1;

    correct_boundary_elems(1, 0) = 1;
    correct_boundary_elems(1, 1) = 2;

    correct_boundary_elems(2, 0) = 2;
    correct_boundary_elems(2, 1) = 3;

    correct_boundary_elems(3, 0) = 3;
    correct_boundary_elems(3, 1) = 4;

    correct_boundary_elems(4, 0) = 4;
    correct_boundary_elems(4, 1) = 5;

    correct_boundary_elems(5, 0) = 5;
    correct_boundary_elems(5, 1) = 6;

    correct_boundary_elems(6, 0) = 6;
    correct_boundary_elems(6, 1) = 7;

    correct_boundary_elems(7, 0) = 7;
    correct_boundary_elems(7, 1) = 8;

    correct_boundary_elems(8, 0) = 8;
    correct_boundary_elems(8, 1) = 9;
    
    correct_boundary_elems(9, 0) = 0;
    correct_boundary_elems(9, 1) = 9;

    // Generate boundary elements of rectangle
    bound_generator.genRect(boundary_verts, boundary_elems, x_dim, y_dim, x_subdiv, y_subdiv);

    REQUIRE(boundary_verts == correct_boundary_verts);
    REQUIRE(boundary_elems == correct_boundary_elems);
  }

  // SECTION("genTriangulatedRect") {

  //   // Boundary Parameters
  //   // Boundary should be too small resulting in exception being thrown
  //   double length = 0.1;
  //   int subdivisions = 10;

  //   //
  //   BoundaryGenerator bound_generator(mesh, surface_mesh, boundary_mesh);
  //   Eigen::MatrixXd boundary_vertices;
  //   Eigen::MatrixXi boundary_elems;

  //   // Generate square made of edge elements
  //   REQUIRE_THROWS_AS(bound_generator.checkBoundary(length),
  //                     std::invalid_argument);
  // }

  
}