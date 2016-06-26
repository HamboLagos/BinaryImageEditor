#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "quad_node.hpp"

using namespace testing;
using ColorValue = QuadNode::ColorValue;

class TestableQuadNode : public Test
{
protected:
    QuadNode sut;
};

class Initialization : public TestableQuadNode { };

TEST_F(Initialization, OnDefaultConstrcution_HasSaneDefaultsButIsInvalid)
{
    EXPECT_EQ(0, sut.get_side_length());
    EXPECT_EQ(ColorValue::Mixed, sut.get_color_value());

    EXPECT_FALSE(sut.is_valid());
}

TEST_F(Initialization, AfterInitialization_NodeIsValid)
{
    sut.init(42, ColorValue::White);

    EXPECT_EQ(42, sut.get_side_length());
    EXPECT_EQ(ColorValue::White, sut.get_color_value());

    EXPECT_TRUE(sut.is_valid());
}

TEST_F(Initialization, OnSpecializedConstruction_NodeIsValid)
{
    QuadNode initialized(42, ColorValue::Black);
    EXPECT_TRUE(initialized.is_valid());
}

class Comparison : public TestableQuadNode
{
protected:
    QuadNode one;
    QuadNode other;
};

TEST_F(Comparison, AValidNodeIsAlwaysEqualToItself)
{
    one.init(10, ColorValue::White);
    EXPECT_EQ(one, one);
}

TEST_F(Comparison, GivenTwoNodes)
{
    // both invalid
    EXPECT_NE(one, other);

    // other invalid
    one.init(10, ColorValue::White);
    EXPECT_NE(one, other);

    // both valid and unequal
    other.init(11, ColorValue::White);
    EXPECT_NE(one, other);

    // both valid and equal
    other.init(10, ColorValue::White);
    EXPECT_EQ(one, other);

}

class AddingChildren : public TestableQuadNode
{
protected:
    static constexpr size_t SIDE_LENGTH = 10;
    static constexpr ColorValue COLOR   = ColorValue::Black;

    QuadNode::Quad<std::unique_ptr<QuadNode>> children;

    AddingChildren() :
        children({
            std::unique_ptr<QuadNode>(new QuadNode(SIDE_LENGTH, COLOR)),
            std::unique_ptr<QuadNode>(new QuadNode(SIDE_LENGTH, COLOR)),
            std::unique_ptr<QuadNode>(new QuadNode(SIDE_LENGTH, COLOR)),
            std::unique_ptr<QuadNode>(new QuadNode(SIDE_LENGTH, COLOR))
        })
    {
        sut.init(SIDE_LENGTH, COLOR);
    }
};
constexpr size_t AddingChildren::SIDE_LENGTH;
constexpr ColorValue AddingChildren::COLOR;

TEST_F(AddingChildren, OnConstrcution_HasNoChildren)
{
    EXPECT_TRUE(sut.is_leaf());
}

TEST_F(AddingChildren, AfterChildrenAdded_IsNoLongerALeafNode)
{
    auto ret = sut.set_children(std::move(children));

    EXPECT_TRUE(ret);
    EXPECT_TRUE(sut.is_valid());
    EXPECT_FALSE(sut.is_leaf());
}

TEST_F(AddingChildren, NodeIsValidEvenIfOneOfTheChildrenIsUnitialized)
{
    children.q3.reset(new QuadNode());

    // invalid because one of the children is no longer set
    auto ret = sut.set_children(std::move(children));

    EXPECT_TRUE(ret);
    EXPECT_TRUE(sut.is_valid());
    EXPECT_FALSE(sut.is_leaf());
}
TEST_F(AddingChildren, NodeIsInvalidButBecomesALeafWhenOneOfTheChildrenIsNull)
{
    children.q3.reset();

    auto ret = sut.set_children(std::move(children));

    EXPECT_FALSE(ret);
    EXPECT_TRUE(sut.is_valid());
    EXPECT_TRUE(sut.is_leaf());
}

class DeletingNodes : public AddingChildren
{
protected:
    DeletingNodes()
    {
        sut.init(SIDE_LENGTH, COLOR);
        sut.set_children(std::move(children));
    }
};
