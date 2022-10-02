#include "pythonTools.hpp"


void createBound(std::string filepath)
{
    std::string pyCommStr = "python3 ./pythonScripts/createBound_general.py " + filepath;
    const char* pyComm = pyCommStr.c_str();
    system(pyComm);
}

void convertMesh(std::string filepath, std::string extension)
{
    std::string pyCommStr = "python3 ./pythonScripts/convertMesh.py " + filepath + " " + extension;
    const char* pyComm = pyCommStr.c_str();
    system(pyComm);
}