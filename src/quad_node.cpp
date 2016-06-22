#include "quad_node.hpp"

QuadNode::QuadNode() :
    side_length(0)
{ }

size_t
QuadNode::get_side_length() const
{
    return side_length;
}

QuadNode::ColorValue
QuadNode::get_color_value() const
{
    return QuadNode::ColorValue::Black;
}

bool
QuadNode::is_valid() const
{
    return false;
}
