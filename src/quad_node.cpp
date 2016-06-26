#include "quad_node.hpp"

QuadNode::QuadNode(size_t side_length, ColorValue color) :
    side_length_(side_length),
    color_(color),
    was_initialized_(true)
{ }

QuadNode::QuadNode() :
    side_length_(0),
    color_(ColorValue::Mixed),
    was_initialized_(false)
{ }

void
QuadNode::init(size_t side_length, ColorValue color)
{
    side_length_ = side_length;
    color_ = color;
    was_initialized_ = true;
}

size_t
QuadNode::get_side_length() const
{
    return side_length_;
}

QuadNode::ColorValue
QuadNode::get_color_value() const
{
    return color_;
}

bool
QuadNode::is_leaf() const
{
    return
        children_.q1.use_count() == 0 &&
        children_.q2.use_count() == 0 &&
        children_.q3.use_count() == 0 &&
        children_.q4.use_count() == 0;
}

bool
QuadNode::is_valid() const
{
    if(!was_initialized_) {
        return false;
    }

    return is_leaf() || has_valid_children();
}

bool
QuadNode::set_children(Quad<std::unique_ptr<QuadNode>> children)
{
    // use move semantics to obtain ownership of the children
    children_.q1 = std::move(children.q1);
    children_.q2 = std::move(children.q2);
    children_.q3 = std::move(children.q3);
    children_.q4 = std::move(children.q4);

    // if any of the children are invalid, don't use them
    // it wouldn't make sense to keep our old children either, so we reset all
    if (!has_valid_children()) {
        children_.q1.reset();
        children_.q2.reset();
        children_.q3.reset();
        children_.q4.reset();
        return false;
    }

    return true;
}

QuadNode::Quad<std::shared_ptr<QuadNode>>
QuadNode::get_children() const
{
    return Quad<std::shared_ptr<QuadNode>>({
        children_.q1,
        children_.q2,
        children_.q3,
        children_.q4,
    });
}

bool
QuadNode::has_valid_children() const
{
    return
      children_.q1 && children_.q1.use_count() != 0 &&
      children_.q2 && children_.q2.use_count() != 0 &&
      children_.q3 && children_.q3.use_count() != 0 &&
      children_.q4 && children_.q4.use_count() != 0;
}

bool
QuadNode::operator==(const QuadNode& other) const
{
    if (!is_valid() || !other.is_valid()) {
        return false;
    }

    return
        side_length_ == other.side_length_ &&
        color_       == other.color_;
}

bool
QuadNode::operator!=(const QuadNode& other) const
{
    return !(*this == other);
}
