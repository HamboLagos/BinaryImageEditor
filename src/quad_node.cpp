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

void
QuadNode::set_children(const Children& children)
{
    // assignment operator on shared pointers adds the assigned pointer as an owner,
    // ie. this will increase the ref count, making us a shared owner, which is the correct behavior
    this->children_ = children;
}

const QuadNode::Children
QuadNode::get_children() const
{
    return children_;
}

bool
QuadNode::has_valid_children() const
{
        return
            children_.q1.use_count() != 0 &&
            children_.q2.use_count() != 0 &&
            children_.q3.use_count() != 0 &&
            children_.q4.use_count() != 0;
}

bool
QuadNode::operator==(const QuadNode& other) const
{
    if (!is_valid() || !other.is_valid()) {
        return false;
    }

    return
        side_length_     == other.side_length_     &&
        color_           == other.color_;
}

bool
QuadNode::operator!=(const QuadNode& other) const
{
    return !(*this == other);
}
