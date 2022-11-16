#include"meshBoolean.hpp"

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef typename CGAL::Polyhedron_3<K, CGAL::Polyhedron_items_with_id_3> Polyhedron;
typedef typename CGAL::Nef_polyhedron_3<K> Nef_Polyhedron;

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
  Nef_Polyhedron np1(geom);
  Nef_Polyhedron np2(bound);

  Nef_Polyhedron intersect = np2 * np1;
  Nef_Polyhedron up2 = np2 - intersect;
  Polyhedron update2;
  up2.convert_to_polyhedron(update2);
  up2.clear();

  // Just the coil
  Nef_Polyhedron up1 = np1 - intersect;
  Polyhedron update1;
  up1.convert_to_polyhedron(update1);
  up1.clear();
    
  np1.clear();
  np2.clear();

  Polyhedron difference;
  intersect.convert_to_polyhedron(difference);
  intersect.clear(); 
  return update2;
}
