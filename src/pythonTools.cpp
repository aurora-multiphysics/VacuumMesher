#include "pythonTools.hpp"


void createBound()
{
    std::string pyCommStr = "python3 ./pythonScripts/createBound_general.py ";
    const char* pyComm = pyCommStr.c_str();
    system(pyComm);
}

void convertMesh(std::string filepath, char extension)
{
    std::string pyCommStr = "python3 ./pythonScripts/convertMesh.py " + filepath + " " + extension;
    const char* pyComm = pyCommStr.c_str();
    system(pyComm);
}