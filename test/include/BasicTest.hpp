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
#include "algorithm"
#include <filesystem>


class BasicTest : public ::testing::Test {
 protected:
    BasicTest()
    {
    }

    virtual void SetUp() override{};
};