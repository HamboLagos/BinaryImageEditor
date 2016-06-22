#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "quad_node.hpp"

using namespace testing;
using ColorValue = QuadNode::ColorValue;

class TestableQuadNode : public Test
{
protected:
    QuadNode sut;

    auto is_valid() -> decltype(sut.is_valid()) { return sut.is_valid(); }
};

class DefaultConstructor : public TestableQuadNode { };

TEST_F(DefaultConstructor, HasSaneDefaultsButIsInvalid)
{
    EXPECT_EQ(0, sut.get_side_length());
    EXPECT_EQ(ColorValue::Black, sut.get_color_value());
    EXPECT_FALSE(is_valid());
}
