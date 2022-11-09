#include"meshBoolean.hpp"

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef typename CGAL::Polyhedron_3<K, CGAL::Polyhedron_items_with_id_3> Polyhedron;
typedef typename CGAL::Nef_polyhedron_3<K> Nef_Polyhedron;

void genBooleanBound(libMesh::Mesh& boundaryMesh, 
                     libMesh::Mesh& surfaceMesh, 
                     libMesh::Mesh& mesh)
{   
    
    // Normals for visualising
    // Eigen::MatrixXd N_faces;

    // Eigen::MatrixXd VA,VB,VC;
    // Eigen::MatrixXi FA, FB, FC;  
    // Eigen::VectorXi J,surfaceMeshI;
    // libMeshToIGL(surfaceMesh, VA, FA);
    // libMeshToIGL(boundaryMesh, VB, FB);

    // std::vector<libMesh::dof_id_type> element_id;
    // std::vector<short unsigned int> side_id;
    // std::vector<libMesh::boundary_id_type> bd_id;
    // surfaceMesh.get_boundary_info().build_side_list(element_id, side_id, bd_id);
    // for(int i = 0; i<FA.rows(); i++)
    // {   
    //     if(!std::binary_search(element_id.begin(), element_id.end(), i))
    //     {
    //         reverseNormal(i, FA);
    //     }
    // }

    // igl::MeshBooleanType boolean_type(igl::MESH_BOOLEAN_TYPE_UNION);

    // igl::copyleft::cgal::mesh_boolean(VA,FA,VB,FB,boolean_type,VC,FC,J);
    // IGLToLibMesh(mesh, VC, FC);    
    
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
