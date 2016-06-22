#include "quad_tree.hpp"

std::shared_ptr<const QuadNode>
QuadTree::get_root() const
{
    return root;
}

bool
QuadTree::is_valid() const
{
    /// TODO actually check the tree is valid
    return false;
}
