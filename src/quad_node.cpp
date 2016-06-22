#include "quad_node.hpp"

QuadNode::QuadNode() :
    side_length(0),
    color(ColorValue::Black),
    valid(false)
{ }

QuadNode::QuadNode(size_t side_length, ColorValue color) :
    side_length(side_length),
    color(color),
    valid(true)
{ }

void
QuadNode::init(size_t side_length, ColorValue color)
{
    this->side_length = side_length;
    this->color = color;
    valid = true;
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
QuadNode::is_valid() const
{
    return valid;
}
