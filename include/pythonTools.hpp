#pragma once
#include<iostream>
#include <chrono>
#include <unistd.h>
#include <filesystem>

void createBound(std::string filepath);

void convertMesh(std::string filepath, std::string extension);