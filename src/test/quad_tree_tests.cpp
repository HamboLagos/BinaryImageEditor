#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "quad_tree.hpp"

using namespace testing;

class TestableQuadTree : public Test
{
protected:
    QuadTree sut;
};

class Unitialized : public TestableQuadTree { };

TEST_F(Unitialized, TreeIsInvalid)
{
    EXPECT_FALSE(sut.is_valid());
}
