#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "quad_tree.hpp"

using namespace testing;
using ColorValue = QuadNode::ColorValue;

class TestableQuadTree : public Test
{
protected:
    QuadTree sut;

    void set_root(QuadTree& tree, std::shared_ptr<QuadNode> root) { tree.set_root(root); }
    auto parse_rows(const std::vector<bool>& data, size_t side_length)
        -> decltype(QuadTree::parse_rows(data, side_length))
    {
        return QuadTree::parse_rows(data, side_length);
    }
};

class Unitialized : public TestableQuadTree { };

TEST_F(Unitialized, TreeIsInvalid)
{
    EXPECT_FALSE(sut.is_valid());
}

class Initialization : public TestableQuadTree
{

protected:
    std::vector<bool> data;
};

TEST_F(Initialization, GivenSinglePixel_CreatesTreeWithDepth0)
{
    data = {false};
    sut.init(data, 1);

    EXPECT_TRUE(sut.is_valid());

    QuadTree expected;
    EXPECT_NE(expected, sut);

    set_root(expected, std::make_shared<QuadNode>(1, ColorValue::White));
    EXPECT_EQ(expected, sut);
}

TEST_F(Initialization, Given4x4HomogenousImage_CreatesTreeWithDepth0)
{
    data = {
        true, true,
        true, true
    };
    sut.init(data, 2);

    EXPECT_TRUE(sut.is_valid());

    QuadTree expected;
    set_root(expected, std::make_shared<QuadNode>(2, ColorValue::Black));
    EXPECT_EQ(expected, sut);
}

TEST_F(Initialization, Given4x4HeterogeneousImage_CreatesTreeWithDepth1)
{
    data = {
        false, true ,
        true , false
    };
    sut.init(data, 2);

    EXPECT_TRUE(sut.is_valid());

    QuadTree expected;
    std::shared_ptr<QuadNode> root = std::make_shared<QuadNode>(2, ColorValue::Mixed);
    set_root(expected, root);
    EXPECT_NE(expected, sut);

    QuadNode::Children l1 = {
        std::make_shared<QuadNode>(1, ColorValue::Black),
        std::make_shared<QuadNode>(1, ColorValue::White),
        std::make_shared<QuadNode>(1, ColorValue::Black),
        std::make_shared<QuadNode>(1, ColorValue::White)
    };
    root->set_children(l1);
    EXPECT_EQ(expected, sut);
}

class ParseRows : public TestableQuadTree { };

TEST_F(ParseRows, GivenXByYInput_ReturnsXByYOutput)
{
    // 1x1 input
    std::vector<bool> data = {true};
    auto rows = parse_rows(data, 1);

    decltype(rows) expected;
    expected.push_back(std::vector<bool>({true}));

    EXPECT_EQ(expected, rows);

    // 2x2 input
    data = {true, true, false, false};
    rows = parse_rows(data, 2);

    std::vector<bool> first = {true, true};
    std::vector<bool> second = {false, false};
    expected = {first, second};

    EXPECT_EQ(expected, rows);
}

class Comparison : public TestableQuadTree
{
protected:
    QuadTree one;
    QuadTree other;
};

TEST_F(Comparison, ATreeIsAlwaysEqualToItself)
{
    EXPECT_EQ(one, one);

    one.init({true}, 1);
    EXPECT_EQ(one, one);
}

TEST_F(Comparison, GivenTwoNodes)
{
    EXPECT_EQ(one, other);

    one.init({true}, 1);
    EXPECT_NE(one, other);

    other.init({true}, 1);
    EXPECT_EQ(one, other);

    one.init({true, false, true, false}, 2);
    EXPECT_NE(one, other);

    other.init({true, false, true, false}, 2);
    EXPECT_EQ(one, other);

    other.init({false, false, true, false}, 2);
    EXPECT_NE(one, other);
}
