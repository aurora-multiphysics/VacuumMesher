#include"BasicTest.hpp"

class MeshTest : public BasicTest {
    
 protected:
    MeshTest(std::string meshFile) : meshFileName(meshFile), BasicTest()
    {

    }

    void setMesh(){mesh->read(meshFileName);}

    virtual void SetUp() override
    {
        std::cout << "createLibEnv" << std::endl;
        createLibmeshEnv();
        std::cout << "NextBit" << std::endl;
        setMesh();
        // mesh->print_info();
    }

    std::string meshFileName;
};