#include"BasicTest.hpp"
#include"surfaceMeshing.hpp"

class MeshTest : public BasicTest {
    
 protected:
    MeshTest(std::string meshFile) : meshFileName(meshFile), BasicTest()
    {

    }

    void setMesh()
    {
        mesh->read(meshFileName);
        // getSurface(mesh, surfaceMesh, elements);
    }

    virtual void SetUp() override
    {
        createLibmeshEnv();
        setMesh();
    }

    std::string meshFileName;

    //Libmesh Mesh skin object
    libMesh::Mesh* surfaceMesh;
};