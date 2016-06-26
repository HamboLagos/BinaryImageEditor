#include "quad_tree.hpp"

#include <iostream>
#include <cmath>

using ColorValue = QuadNode::ColorValue;

QuadTree::QuadTree() = default;

void
QuadTree::init(const Data& data)
{
    root_.reset();

    auto image_area = data.size();
    if (image_area == 0) {
        return;
    }

    // check that data.size() is a square number, ie. the image is square
    auto side_length = sqrt(image_area);
    bool is_square_image = (side_length == ceil(side_length));
    if (!is_square_image) {
        return;
    }

    Rows rows = parse_rows(data);
    auto root = make_node(rows);

    // if its not a homogenous image, we have more work to do
    if (root->get_color_value() == ColorValue::Mixed) {
        init_recursive(rows, *root);
    }

    root_ = std::move(root);
}

bool
QuadTree::is_valid() const
{
    bool root_initialized = root_.use_count() != 0;
    bool root_valid = root_initialized && root_ && root_->is_valid();

    if (!root_valid) {
        return false;
    }

    if (root_->is_leaf()) {
        return true;
    }

    auto children = root_->get_children();
    return
        QuadTree(children.q1).is_valid() &&
        QuadTree(children.q2).is_valid() &&
        QuadTree(children.q3).is_valid() &&
        QuadTree(children.q4).is_valid();
}

bool
QuadTree::operator==(const QuadTree& other) const
{
    if (!is_valid() || !other.is_valid()) {
        return false;
    }

    // if we get here, we know both tree roots are initialized
    if (*root_ != *other.root_) {
        return false;
    }

    if (root_->is_leaf() && other.root_->is_leaf()) {
        return true;
    }

    if (root_->is_leaf() != other.root_->is_leaf()) {
        return false;
    }

    auto children       = root_->get_children();
    auto other_children = other.root_->get_children();

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

QuadTree::QuadTree(std::shared_ptr<QuadNode> root)
{
    this->root_ = root;
}

QuadTree::Rows
QuadTree::parse_rows(const Data& data)
{
    auto side_length = sqrt(data.size());
    Rows rows;
    rows.reserve(side_length);

    for (size_t row = 0; row < side_length; ++row) {
        auto row_start = data.begin() + row * side_length;
        auto row_end = row_start + side_length;

        rows.emplace_back(row_start, row_end);
    }

    return rows;
}

QuadTree::Quad<QuadTree::Rows>
QuadTree::get_quadrants(const Rows& rows)
{
    auto quad_side_length = rows.size() / 2;
    Quad<Rows> quadrants;

    quadrants.q1 = parse_quadrant(rows, quad_side_length, 0               );
    quadrants.q2 = parse_quadrant(rows, 0               , 0               );
    quadrants.q3 = parse_quadrant(rows, 0               , quad_side_length);
    quadrants.q4 = parse_quadrant(rows, quad_side_length, quad_side_length);

    return quadrants;
}

QuadTree::Rows
QuadTree::parse_quadrant(const Rows& rows, size_t x_off, size_t y_off)
{
    auto side_length = rows.size() / 2;
    Rows quadrant;
    quadrant.reserve(side_length);

    for (size_t row = 0; row < side_length; ++row) {
        auto row_start = rows.at(y_off + row).begin() + x_off;
        auto row_end = row_start + side_length;

        quadrant.emplace_back(row_start, row_end);
    }

    return quadrant;
}

QuadTree::Quad<std::unique_ptr<QuadNode>>
QuadTree::make_nodes(const Quad<Rows>& quadrants)
{
    auto q1 = make_node(quadrants.q1);
    auto q2 = make_node(quadrants.q2);
    auto q3 = make_node(quadrants.q3);
    auto q4 = make_node(quadrants.q4);

    return {
        std::move(q1),
        std::move(q2),
        std::move(q3),
        std::move(q4)
    };
}

void
QuadTree::init_recursive(const Rows& rows, QuadNode& parent)
{
    auto quadrants = get_quadrants(rows);
    auto nodes = make_nodes(quadrants);

    if (nodes.q1->get_color_value() == ColorValue::Mixed) {
        init_recursive(quadrants.q1, *nodes.q1);
    }

    if (nodes.q2->get_color_value() == ColorValue::Mixed) {
        init_recursive(quadrants.q2, *nodes.q2);
    }

    if (nodes.q3->get_color_value() == ColorValue::Mixed) {
        init_recursive(quadrants.q3, *nodes.q3);
    }

    if (nodes.q4->get_color_value() == ColorValue::Mixed) {
        init_recursive(quadrants.q4, *nodes.q4);
    }

    parent.set_children(std::move(nodes));
}

std::unique_ptr<QuadNode>
QuadTree::make_node(const Rows& rows)
{
    auto side_length = rows.size();

    auto first_color = rows.at(0).at(0);
    bool homogenous = true;
    for(auto row : rows) {
        for(auto pixel : row) {
            if(pixel != first_color) {
                homogenous = false;
                break;
            }
        }
    }

    if(homogenous) {
        return std::unique_ptr<QuadNode>(new QuadNode(side_length, first_color));
    }

    return std::unique_ptr<QuadNode>(new QuadNode(side_length, ColorValue::Mixed));
}
