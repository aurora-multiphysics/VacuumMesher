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


// Global hits
std::vector<int> tree_hits;

// callback function for rtree
bool tree_callback(int id) {  
  tree_hits.push_back(id);
  return true;
}

void genBooleanBound(libMesh::Mesh& boundaryMesh, 
                     libMesh::Mesh& surfaceMesh)
{   
  Polyhedron polGeom;
  Polyhedron polBound;
  
  libmeshToCGAL(surfaceMesh, polGeom);
  std::cout << "converted surface" << std::endl;
  libmeshToCGAL(boundaryMesh, polBound);
  std::cout << "converted bound" << std::endl;
  Polyhedron difference = subtract_volumes_poly(polGeom, polBound);
  CGALToLibmesh(surfaceMesh, difference);
  // removeDegenerateTris(boundaryMesh, 1e-07);
}

// Use CGAL to generate the boundary for the coil
Polyhedron 
subtract_volumes_poly(Polyhedron& geom, Polyhedron& bound)
{    
  Nef_Polyhedron np1(geom);
  Nef_Polyhedron np2(bound);

  Nef_Polyhedron subtraction = np2 - np1;
  Polyhedron booleanBound;
  subtraction.convert_to_Polyhedron(booleanBound);

  std::ofstream fileOut("CGAL.off");
  CGAL::IO::write_OFF(fileOut, booleanBound);
  np1.clear();
  np2.clear();
  
  return booleanBound;
}

void
removeDegenerateTris(libMesh::Mesh& cgalMesh, const double& tol)
{
  // copy of mesh to work with
  libMesh::Mesh meshCopy(cgalMesh);
  // Clear original mesh
  cgalMesh.clear();
  // Map to keep track of 
  std::unordered_map<unsigned int, unsigned int> id_map;
  RTree<int, double, 3, float> rtree;

  for(auto& node: meshCopy.local_node_ptr_range())
  {
    tree_hits.clear();
    std::array node_coords = {(*node)(0), (*node)(1), (*node)(2)};
    Box node_box = Box(node_coords, tol, node->id());
    int nhits = rtree.Search(node_box.min.data(), node_box.max.data(), tree_callback);
    if(!nhits)
    {
      id_map[node->id()] =  node->id();
      rtree.Insert(node_box.min.data(), node_box.max.data(), node_box.node_id);
      double pnt[3];
      for(uint i = 0; i<3; i++){pnt[i] = (*node)(i);}
      libMesh::Point xyz(pnt[0], pnt[1], pnt [2]);
      cgalMesh.add_point(xyz, id_map[node->id()]);
    }
    else{
      id_map[node->id()] =  tree_hits[0];
    }
  }

  // add Elements that have non-zero area (well, techincally, non-reallysmallvalue area),
  // because precision 

  for(auto& elem: libMesh::as_range(meshCopy.local_elements_begin(), meshCopy.local_elements_end()))
  {
    if(elem->volume() > tol)
    {
      libMesh::Elem* new_elem = libMesh::Elem::build(elem->type()).release();
      for(int j = 0; j < new_elem->n_nodes(); j++)
      {
        new_elem->set_node(j) = cgalMesh.node_ptr(id_map[elem->node_ptr(j)->id()]);
      }
      cgalMesh.add_elem(new_elem);
    }
  } 
  cgalMesh.prepare_for_use();
}