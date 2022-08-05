#include "BasicTest.hpp"
#include "surfaceMeshing.hpp"

class MeshTest : public BasicTest
{

protected:
    MeshTest(std::string meshFile) : BasicTest(), meshFileName(meshFile)
    {
    }

    void setMesh()
    {   
        surfaceMesh = std::make_shared<libMesh::Mesh>(init->comm());
        std::set<int> elems;
        for(int i = 0; i < 75613; ++i)
        {
            elems.emplace_hint(elems.end(), i);
        }
        
        mesh->read(meshFileName);
        getSurface(*mesh, *surfaceMesh, elems);
    }

    virtual void SetUp() override
    {
        createLibmeshEnv();
        setMesh();
    }

    std::string meshFileName;

    // Libmesh Mesh skin object
    std::shared_ptr<libMesh::Mesh> surfaceMesh = nullptr;
};