#include "quad_node.hpp"

#include <iostream>

QuadNode::QuadNode() :
    side_length(0),
    color(ColorValue::Mixed),
    was_initialized(false)
{ }

QuadNode::QuadNode(size_t side_length, ColorValue color) :
    side_length(side_length),
    color(color),
    was_initialized(true)
{ }

void
QuadNode::init(size_t side_length, ColorValue color)
{
    this->side_length = side_length;
    this->color = color;
    was_initialized = true;
}

size_t
QuadNode::get_side_length() const
{
    return side_length;
}

QuadNode::ColorValue
QuadNode::get_color_value() const
{
    return color;
}

bool
QuadNode::is_leaf() const
{
    return
        children.q1.use_count() == 0 &&
        children.q2.use_count() == 0 &&
        children.q3.use_count() == 0 &&
        children.q4.use_count() == 0;
}

bool
QuadNode::are_children_valid(const Children& children) const
{
        return
            children.q1.use_count() != 0 && children.q1->is_valid() &&
            children.q2.use_count() != 0 && children.q2->is_valid() &&
            children.q3.use_count() != 0 && children.q3->is_valid() &&
            children.q4.use_count() != 0 && children.q4->is_valid();
}

bool
QuadNode::is_valid() const
{
    if(!was_initialized) {
        return false;
    }

    return is_leaf() || are_children_valid(children);
}

void
QuadNode::set_children(const Children& children)
{
    // assignment operator on shared pointers adds the assigned pointer as an owner,
    // ie. this will increase the ref count, which is the correct behavior
    this->children = children;
}

const QuadNode::Children
QuadNode::get_children() const
{
    return children;
}
