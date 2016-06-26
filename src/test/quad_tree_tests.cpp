#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "quad_tree.hpp"

using namespace testing;
using C = QuadNode::ColorValue;

class TestableQuadTree : public Test
{
protected:
    QuadTree sut;

    QuadTree tree_from_root(std::shared_ptr<QuadNode> root) { return QuadTree(root); }
};

class Initialization : public TestableQuadTree { };

TEST_F(Initialization, Unitialized_TreeIsInvalid)
{
    EXPECT_FALSE(sut.is_valid());
}

TEST_F(Initialization, GivenEmptyData_TreeIsInvalid)
{
    sut.init({ });
    EXPECT_FALSE(sut.is_valid());
}

TEST_F(Initialization, GivenNonSquareData_TreeIsInvalid)
{
    sut.init({C::White});
    EXPECT_TRUE(sut.is_valid());

    sut.init({C::White, C::White});
    EXPECT_FALSE(sut.is_valid());

    sut.init({
        C::White, C::White,
        C::Black});
    EXPECT_FALSE(sut.is_valid());

    sut.init({
        C::White, C::White,
        C::Black, C::Black});
    EXPECT_TRUE(sut.is_valid());

}

TEST_F(Initialization, GivenSinglePixel_CreatesTreeWithDepth0)
{
    sut.init({C::White});
    EXPECT_TRUE(sut.is_valid());

    auto expected = tree_from_root(std::make_shared<QuadNode>(1, C::White));
    EXPECT_TRUE(expected.is_valid());

    EXPECT_EQ(expected, sut);
}

TEST_F(Initialization, Given2x2HomogenousImage_CreatesTreeWithDepth0)
{
    sut.init({
        C::White, C::White,
        C::White, C::White
    });
    EXPECT_TRUE(sut.is_valid());

    auto expected = tree_from_root(std::make_shared<QuadNode>(2, C::White));
    EXPECT_TRUE(expected.is_valid());
    EXPECT_EQ(expected, sut);
}

TEST_F(Initialization, Given2x2HeterogeneousImage_CreatesTreeWithDepth1)
{
    sut.init({
        C::White, C::Black,
        C::Black, C::White
    });
    EXPECT_TRUE(sut.is_valid());

    QuadNode::Quad<std::unique_ptr<QuadNode>> l1 = {
        std::unique_ptr<QuadNode>(new QuadNode(1, C::Black)),
        std::unique_ptr<QuadNode>(new QuadNode(1, C::White)),
        std::unique_ptr<QuadNode>(new QuadNode(1, C::Black)),
        std::unique_ptr<QuadNode>(new QuadNode(1, C::White))
    };
    auto root = std::make_shared<QuadNode>(2, C::Mixed);
    root->set_children(std::move(l1));

    auto expected = tree_from_root(root);
    EXPECT_TRUE(expected.is_valid());
    EXPECT_EQ(expected, sut);
}

TEST_F(Initialization, Given4x4HeterogeneousImage_CreatesTreeWithDepth1)
{
    sut.init({
        C::White, C::White, C::Black, C::White,
        C::White, C::White, C::White, C::Black,
        C::Black, C::Black, C::Black, C::Black,
        C::White, C::White, C::Black, C::Black
    });
    EXPECT_TRUE(sut.is_valid());

    QuadNode::Quad<std::unique_ptr<QuadNode>> root_children = {
        std::unique_ptr<QuadNode>(new QuadNode(2, C::Mixed)),
        std::unique_ptr<QuadNode>(new QuadNode(2, C::White)),
        std::unique_ptr<QuadNode>(new QuadNode(2, C::Mixed)),
        std::unique_ptr<QuadNode>(new QuadNode(2, C::Black))
    };

    QuadNode::Quad<std::unique_ptr<QuadNode>> root_q1_children = {
        std::unique_ptr<QuadNode>(new QuadNode(1, C::White)),
        std::unique_ptr<QuadNode>(new QuadNode(1, C::Black)),
        std::unique_ptr<QuadNode>(new QuadNode(1, C::White)),
        std::unique_ptr<QuadNode>(new QuadNode(1, C::Black))
    };
    root_children.q1->set_children(std::move(root_q1_children));

    QuadNode::Quad<std::unique_ptr<QuadNode>> root_q3_children = {
        std::unique_ptr<QuadNode>(new QuadNode(1, C::Black)),
        std::unique_ptr<QuadNode>(new QuadNode(1, C::Black)),
        std::unique_ptr<QuadNode>(new QuadNode(1, C::White)),
        std::unique_ptr<QuadNode>(new QuadNode(1, C::White))
    };
    root_children.q3->set_children(std::move(root_q3_children));

    auto root = std::make_shared<QuadNode>(4, C::Mixed);
    root->set_children(std::move(root_children));

    auto expected = tree_from_root(root);
    EXPECT_TRUE(expected.is_valid());
    EXPECT_EQ(expected, sut);
}

class Comparison : public TestableQuadTree
{
protected:
    QuadTree one;
    QuadTree other;
};

TEST_F(Comparison, AValidTreeIsAlwaysEqualToItself)
{
    one.init({C::White});
    EXPECT_TRUE(one.is_valid());

    EXPECT_EQ(one, one);
}

TEST_F(Comparison, GivenTwo1x1Trees)
{
    // both invalid
    EXPECT_NE(one, other);

    // other invalid
    one.init({C::White});
    EXPECT_NE(one, other);

    // both valid and unequal
    other.init({C::Black});
    EXPECT_NE(one, other);

    // both valid and equal
    other.init({C::White});
    EXPECT_EQ(one, other);
}

TEST_F(Comparison, GivenTwo2x2Trees)
{
    // both valid and unequal
    one.init({C::Black, C::Black, C::White, C::White});
    EXPECT_NE(one, other);

    // both valid and unequal
    other.init({C::Black, C::Black, C::Black, C::White});
    EXPECT_NE(one, other);

    // both valid and equal
    other.init({C::Black, C::Black, C::White, C::White});
    EXPECT_EQ(one, other);
}

TEST_F(Comparison, GivenTwo4x4Trees)
{
    one.init({
        C::Black, C::Black, C::White, C::White,
        C::Black, C::Black, C::Black, C::Black,
        C::White, C::Black, C::White, C::White,
        C::White, C::Black, C::White, C::White,
    });
    EXPECT_TRUE(one.is_valid());

    other.init({
        C::Black, C::Black, C::White, C::White,
        C::Black, C::Black, C::Black, C::Black,
        C::White, C::Black, C::White, C::White,
        C::White, C::Black, C::White, C::White,
    });
    EXPECT_TRUE(other.is_valid());

    EXPECT_EQ(one, other);
}
