#include "MeshTest.hpp"
#include "surfaceMeshing.hpp"

class SkinningTest : public MeshTest
{

protected:
    SkinningTest(std::string meshFile) : MeshTest(meshFile)
    {
    }


    virtual void SetUp() override
    {
        createLibmeshEnv();
        setMesh();
    }


    void getFilePaths()
    {
        cubitSkinFilename = "../Meshes/cubitSkins/" + meshFilename.substr(0, meshFilename.find(".")) + "_cub_skinned.e"; 
        meshFilename = meshFilename.substr(0, meshFilename.find("."));
        meshFilename = "../Meshes/" + meshFilename + ".e"; 
    }
    
    virtual void setMesh() override
    {   
        getFilePaths();
        MeshTest::setMesh();


        surfaceMesh = std::make_shared<libMesh::Mesh>(init->comm());
        cubitSurfaceMesh = std::make_shared<libMesh::Mesh>(init->comm());
        cubitSurfaceMesh->read(cubitSkinFilename);


        std::set<int> elems;
        for(int i = 0; i < mesh->n_elem(); ++i)
        {
            elems.emplace_hint(elems.end(), i);
        }
        getSurface(*mesh, *surfaceMesh, elems);

        std::cout << cubitSkinFilename << std::endl;
    }


    //File name of the corresponding cubit skinned mesh
    std::string cubitSkinFilename;

    // Libmesh Mesh object to store the skinned mesh
    std::shared_ptr<libMesh::Mesh> surfaceMesh = nullptr;

    // Pointer to libMesh mesh object that contains the cubit skinned mesh 
    std::shared_ptr<libMesh::Mesh> cubitSurfaceMesh = nullptr;
};