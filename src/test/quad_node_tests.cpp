#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "quad_node.hpp"

using namespace testing;
using ColorValue = QuadNode::ColorValue;

class TestableQuadNode : public Test
{
protected:
    QuadNode* sut;

    void construct()
    {
        sut = new QuadNode;
    }

    void construct(size_t side_length, ColorValue color)
    {
        sut = new QuadNode(side_length, color);
    }

    ~TestableQuadNode()
    {
        if(sut != nullptr) {
            delete sut;
        }
    }

    void init(size_t side_length, ColorValue color) { sut->init(side_length, color); }
    bool is_valid() { return sut->is_valid(); }
};

class DefaultConstructorTests : public TestableQuadNode { };

TEST_F(DefaultConstructorTests, HasSaneDefaultsButIsInvalid)
{
    construct();

    EXPECT_EQ(0, sut->get_side_length());
    EXPECT_EQ(ColorValue::Black, sut->get_color_value());
    EXPECT_FALSE(is_valid());
}

TEST_F(DefaultConstructorTests, AfterInitialization_NodeIsValid)
{
    construct();
    init(42, ColorValue::White);

    EXPECT_EQ(42, sut->get_side_length());
    EXPECT_EQ(ColorValue::White, sut->get_color_value());
    EXPECT_TRUE(is_valid());
}

class InitConstructorTests : public TestableQuadNode
{
protected:
};

TEST_F(InitConstructorTests, GetsConstructedWithInitValuesAndIsValid)
{
    construct(42, ColorValue::White);

    EXPECT_EQ(42, sut->get_side_length());
    EXPECT_EQ(ColorValue::White, sut->get_color_value());
    EXPECT_TRUE(is_valid());
}
