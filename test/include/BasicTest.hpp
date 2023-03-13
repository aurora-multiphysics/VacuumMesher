#pragma once
#include <memory>
#include <string>
#include "gtest/gtest.h"
#include "libmesh/mesh.h"
#include "libmesh/libmesh.h"
#include "argsHelper.h"
#include "initer.h"
#include "Tetrahedralisation/tetMaker.hpp"
#include "SurfaceMeshing/surfaceMeshing.hpp"
#include "BoundaryGeneration/boundaryGeneration.hpp"
#include "MeshContainer.hpp"
#include "Tetrahedralisation/removeDupeNodes.hpp"
#include "Utils/parseFlags.hpp"
#include <chrono>
#include "algorithm"
#include <filesystem>


class BasicTest : public ::testing::Test {
 protected:
    BasicTest()
    {
    }

    virtual void SetUp() override{};
};