#pragma once

#include <memory>

#include "quad_node.hpp"

class QuadTree
{
public:
    std::shared_ptr<const QuadNode> get_root() const;

    bool is_valid() const;

    /// TODO define loading from binary data
    /// TODO define interface for applying operations

private:
    std::shared_ptr<const QuadNode> root;
};
