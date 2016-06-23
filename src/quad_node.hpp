#pragma once

#include <cstddef>
#include <memory>

class QuadNode
{
public:
    /** \brief Each node can represent a Black, White, or Mixed-color quadrant. */
    enum class ColorValue {
        Mixed, ///< The quadrant contains both black and white pixels, ie. not a leaf
        Black,
        White
    };

    /** \brief Quadrants are quad-sected using Cartesian Coordinate ordering. */
    struct Children {
        std::shared_ptr<const QuadNode> q1; ///< quadrant 1 child
        std::shared_ptr<const QuadNode> q2; ///< quadrant 2 child
        std::shared_ptr<const QuadNode> q3; ///< quadrant 3 child
        std::shared_ptr<const QuadNode> q4; ///< quadrant 4 child

        inline bool operator==(const Children& other) const
        {
            return
                *q1 == *other.q1 &&
                *q2 == *other.q2 &&
                *q3 == *other.q3 &&
                *q4 == *other.q4;
        }

        inline bool operator!=(const Children& other) const
        {
            return !(*this == other);
        }
    };

    /** \brief Creates and initializes a node */
    QuadNode(size_t side_length, ColorValue color);

    /** \brief Creates an uninitialized node.
     *
     * Nodes are lazily initialized via init(). */
    QuadNode();

    void init(size_t side_length, ColorValue color);

    /** \brief Returns the color associated with this node.
     *
     * This has no real meaning for clients if the node is not a leaf. */
    size_t get_side_length() const;

    /** \brief Returns the color associated with this node.
     *
     * This has no real meaning for clients if the node is not a leaf. */
    ColorValue get_color_value() const;

    /** \brief Returns true iff this node is valid and has no children.
     *
     * If this node is not valid, results are undefined. */
    bool is_leaf() const;

    /** \brief Adds the given nodes as children of this node.
     *
     * Only sets the parent->child relationship, child->parent unimplemented by design. */
    void set_children(const Children& children);

    /** \brief Returns the children of this node.
     *
     * Clients can check if this is a leaf node (ie. has no children) via is_leaf().
     *
     * By returning a copy, we are guaranteeing the lifetime of the children while the caller holds
     * the returned struct. */
    const Children get_children() const;

    /** \brief Returns true iff this node is valid.
     *
     * In this context, valid means the node has been initialized, and has 4 or no children.
     * The validity of each child is not asserted, that responsibility is retained by the parent
     * tree structure */
    bool is_valid() const;

    bool operator==(const QuadNode& other) const;
    bool operator!=(const QuadNode& other) const;

private:
    size_t side_length;
    ColorValue color;
    bool was_initialized;

    Children children;

    /** \brief Children are valid if none are present, or all are present and valid. */
    bool are_children_valid(const Children& children) const;
};
