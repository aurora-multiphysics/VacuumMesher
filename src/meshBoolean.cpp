#include "meshBoolean.hpp"
#include <CGAL/Polygon_mesh_processing/orient_polygon_soup.h>
#include <CGAL/Polygon_mesh_processing/polygon_soup_to_polygon_mesh.h>
#include <CGAL/Polygon_mesh_processing/polygon_mesh_to_polygon_soup.h>
#include <CGAL/Polygon_mesh_processing/orientation.h>
#include <CGAL/polygon_mesh_processing.h>
#include <CGAL/IO/polygon_soup_io.h>
#include <CGAL/Polygon_mesh_processing/corefinement.h>

// TYPEDEFS IMPORTANT FOR CGAL PRECISION
typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef CGAL::Surface_mesh<K::Point_3> Mesh;
typedef std::vector<K::Point_3> Polyline_type;
typedef std::list< Polyline_type > Polylines;
typedef CGAL::AABB_halfedge_graph_segment_primitive<Mesh> HGSP;
typedef CGAL::AABB_traits<K, HGSP>    AABB_traits;
typedef CGAL::AABB_tree<AABB_traits>  AABB_tree;
typedef K::Point_3 Vertex;
typedef CGAL::Polyhedron_3<K, CGAL::Polyhedron_items_with_id_3> Polyhedron;
typedef CGAL::Nef_polyhedron_3<K> Nef_Polyhedron;




void genBooleanBound(libMesh::Mesh& boundaryMesh, 
                     libMesh::Mesh& surfaceMesh, 
                     libMesh::Mesh& mesh)
{   
  Polyhedron polGeom;
  Polyhedron polBound;
  libmeshToCGAL<Polyhedron>(surfaceMesh, polGeom);
  libmeshToCGAL<Polyhedron>(boundaryMesh, polBound);
  Polyhedron difference = subtract_volumes_poly(polGeom, polBound);
  CGALToLibmesh(mesh, difference);
}

// Use CGAL to generate the boundary for the coil
Polyhedron 
subtract_volumes_poly(Polyhedron geom, Polyhedron bound)
{    

  Polyhedron out;
  bool valid_union = CGAL::Polygon_mesh_processing::corefine_and_compute_union(geom,bound, out);

  Nef_Polyhedron np1(geom);
  Nef_Polyhedron np2(bound);

  Nef_Polyhedron subtraction = np1*np2;
  Nef_Polyhedron shape = np2 - subtraction;
  Polyhedron booleanBound;
  Polyhedron booleanBoundOut;
  shape.convert_to_Polyhedron(booleanBound);

  // std::ofstream fileOut("CGAL.off");
  // CGAL::IO::write_OFF(fileOut, booleanBound);
  np1.clear();
  np2.clear();
  

  std::vector<Vertex> points;
  std::vector<std::vector<std::size_t> > polygons;
  

  
  // CGAL::Polygon_mesh_processing::polygon_mesh_to_polygon_soup(booleanBound,points,polygons);
  
  // CGAL::Polygon_mesh_processing::orient_polygon_soup(points,polygons);
  
  // CGAL::Polygon_mesh_processing::merge_duplicate_points_in_polygon_soup(points,polygons);
  // CGAL::Polygon_mesh_processing::merge_duplicate_polygons_in_polygon_soup(points,polygons);
  // CGAL::Polygon_mesh_processing::repair_polygon_soup(points,polygons,
  //                                                    CGAL::parameters::erase_all_duplicates(true).
  //                                                    require_same_orientation(true));
  // CGAL::Polygon_mesh_processing::orient_polygon_soup(points,polygons);
  // CGAL::Polygon_mesh_processing::polygon_soup_to_polygon_mesh(points,polygons,booleanBoundOut);

  CGAL::Polygon_mesh_processing::merge_duplicated_vertices_in_boundary_cycles(booleanBound);
  CGAL::Polygon_mesh_processing::remove_degenerate_edges(booleanBound);
  CGAL::Polygon_mesh_processing::remove_degenerate_faces(booleanBound);

  return booleanBoundOut;
}

void
removeDegenerateTris(libMesh::Mesh cgalMesh)
{
  rTree

}