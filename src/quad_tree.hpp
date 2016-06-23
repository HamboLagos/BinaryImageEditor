#pragma once

#include <memory>
#include <vector>

#include "quad_node.hpp"

class QuadTree
{
friend class TestableQuadTree;
public:

    QuadTree() = default;

    /** \brief Initializes this QuadTree from the given binary data.
     *
     * \param data Binary image data, formatted as a flat array, whose size must be a power of 2 */
    void init(const std::vector<bool>& data, size_t side_length);

    /** \brief Returns true iff the tree is valid.
     *
     * In this context, a tree is valid if it has been initialized, and all its nodes are valid. */
    bool is_valid() const;

    bool operator==(const QuadTree& other) const;
    bool operator!=(const QuadTree& other) const;

    /// TODO define interface for applying operations

private:
    std::shared_ptr<const QuadNode> root;

    /** \brief Does no processing, but sets the given QuadNode as the root of a new tree.
     *
     * Useful for traversing sub-trees. */
    QuadTree(std::shared_ptr<const QuadNode> root);

    /** \brief Test Method: returns tree root. */
    void set_root(std::shared_ptr<const QuadNode> root) { this->root = root; };

    /** \brief Transforms the 1D data array into a 2D structure of dimensions side_length by
     * side_length */
    static std::vector<std::vector<bool>>
        parse_rows(const std::vector<bool>& data, size_t side_length);

    void init_recursive(const std::vector<std::vector<bool>>& rows,
                        size_t x_off, size_t y_off, size_t side_length, QuadNode& parent);

    QuadNode* make_node(std::vector<std::vector<bool>> rows,
                        size_t x_off, size_t y_off, size_t side_length);
};
