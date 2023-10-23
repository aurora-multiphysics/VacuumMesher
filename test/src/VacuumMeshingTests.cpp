// #include "catch2/catch_all.hpp"
// #include "catch2/catch_test_macros.hpp"
// #include "libmesh_test_init.hpp"
// #include "SurfaceMeshing/SurfaceGenerator.hpp"

// TEST_CASE( "SurfaceMeshing", "[surface]" ) {

//     libMesh::Mesh mesh(init.comm());
//     libMesh::Mesh surface_mesh(init.comm());

//     mesh.read(std::string(USER_DIR) +
//     "/test/testingMeshes/unit_cube/unit_cube.e"); SurfaceMeshGenerator
//     surf_generator(mesh, surface_mesh);

//     // SECTION("Check element info is correct"){

//     // }

//     SECTION("getFaceInfo"){
//         std::vector<int> connectivity;
//         std::vector<int> original_node_ids;
//         std::map<int, std::vector<libMesh::boundary_id_type>> boundary_data;
//         int face_id = 0;
//         surf_generator.getFaceInfo(mesh.elem_ptr(0), face_id,
//         original_node_ids, connectivity, boundary_data);

//         REQUIRE(connectivity == std::vector<int>({0, 2, 1}));
//         REQUIRE(original_node_ids == std::vector<int>({0, 2, 1}));

//     }

//     SECTION("isElementSurface"){
//         std::vector<int> surface_faces(6);
//         surf_generator.isElementSurface(mesh.elem_ptr(0), surface_faces);
//         REQUIRE(surface_faces == std::vector<int>{2});

//     }

//     SECTION("getSurface"){
//         surf_generator.getSurface();
//         REQUIRE(surface_mesh.n_elem() == 1404);
//         REQUIRE(surface_mesh.n_nodes() == 704);
//     }
// }