#include "quad_tree.hpp"

#include <iostream>

using ColorValue = QuadNode::ColorValue;

QuadTree::QuadTree(std::shared_ptr<const QuadNode> root)
{
    this->root = root;
}

void
QuadTree::init(const std::vector<bool>& data, size_t side_length)
{
    if (data.size() != side_length * side_length) {
        return;
    }

    ColorValue firstColor = data.at(0) ? ColorValue::Black : ColorValue::White;
    bool homogenous = true;
    for(auto pixel : data) {
        ColorValue color = pixel ? ColorValue::Black : ColorValue::White;

        if(color != firstColor) {
            homogenous = false;
            break;
        }
    }

    if(homogenous) {
        root = std::make_shared<QuadNode>(side_length, firstColor);
        return;
    }

    QuadNode* tree_root = new QuadNode(side_length, ColorValue::Mixed);
    root.reset(tree_root);

    auto rows = parse_rows(data, side_length);
    init_recursive(rows, side_length, 0, 0, *tree_root);
}

void
QuadTree::init_recursive(const std::vector<std::vector<bool>>& rows,
                         size_t side_length, size_t x_off, size_t y_off, QuadNode& parent)
{
    size_t quad_side_length = side_length / 2;

    QuadNode* q1 = make_node(rows, x_off + quad_side_length, y_off                   , quad_side_length);
    QuadNode* q2 = make_node(rows, x_off                   , y_off                   , quad_side_length);
    QuadNode* q3 = make_node(rows, x_off                   , y_off + quad_side_length, quad_side_length);
    QuadNode* q4 = make_node(rows, x_off + quad_side_length, y_off + quad_side_length, quad_side_length);

    QuadNode::Children children = {
        std::shared_ptr<QuadNode>(q1),
        std::shared_ptr<QuadNode>(q2),
        std::shared_ptr<QuadNode>(q3),
        std::shared_ptr<QuadNode>(q4),
    };
    parent.set_children(children);

    if (q1->get_color_value() == ColorValue::Mixed) {
        init_recursive(rows, quad_side_length, x_off + quad_side_length, y_off, *q1);
    }
    if (q2->get_color_value() == ColorValue::Mixed) {
        init_recursive(rows, quad_side_length, x_off, y_off, *q2);
    }
    if (q3->get_color_value() == ColorValue::Mixed) {
        init_recursive(rows, quad_side_length, x_off, y_off + quad_side_length, *q3);
    }
    if (q4->get_color_value() == ColorValue::Mixed) {
        init_recursive(rows, quad_side_length, x_off + quad_side_length, y_off + quad_side_length, *q4);
    }
}

bool
QuadTree::is_valid() const
{
    bool root_initialized = root.use_count() != 0;
    return
        root_initialized &&
        root->is_valid();
}

bool
QuadTree::operator==(const QuadTree& other) const
{
    // if either tree is unitialized, they are only equal if both are unitialized
    if (root.use_count() == 0 || other.root.use_count() == 0) {
        return root.use_count() == other.root.use_count();
    }

    // check the root nodes are equal
    if (*root != *other.root) {
        return false;
    }

    // check the subtree for each child
    QuadNode::Children children       = root->get_children();
    QuadNode::Children other_children = other.root->get_children();

    return
        QuadTree(children.q1) == QuadTree(other_children.q1) &&
        QuadTree(children.q2) == QuadTree(other_children.q2) &&
        QuadTree(children.q3) == QuadTree(other_children.q3) &&
        QuadTree(children.q4) == QuadTree(other_children.q4);
}

bool
QuadTree::operator!=(const QuadTree& other) const
{
    return !(*this == other);
}

std::vector<std::vector<bool>>
QuadTree::parse_rows(const std::vector<bool>& data, size_t side_length)
{
    std::vector<std::vector<bool>> rows;
    rows.reserve(side_length);

    for (size_t row = 0; row < side_length; ++row) {
        auto row_start = data.begin() + row * side_length;
        auto row_end = row_start + side_length;

        rows.emplace_back(row_start, row_end);
    }

    return rows;
}

QuadNode*
QuadTree::make_node(std::vector<std::vector<bool>> rows,
                    size_t x_off, size_t y_off, size_t side_length)
{
    ColorValue firstColor = rows.at(y_off).at(x_off) ? ColorValue::Black : ColorValue::White;

    bool homogenous = true;
    for (size_t row = 0; row < side_length; ++row) {
        auto row_start = rows.at(y_off + row).begin() + x_off;
        auto row_end = row_start + side_length;

        for (auto iter = row_start; iter != row_end; ++iter) {
            ColorValue color = *iter ? ColorValue::Black : ColorValue::White;

            if(color != firstColor) {
                homogenous = false;
                break;
            }
        }
    }

    if (homogenous) {
        return new QuadNode(side_length, firstColor);
    }

    return new QuadNode(side_length, ColorValue::Mixed);
}
