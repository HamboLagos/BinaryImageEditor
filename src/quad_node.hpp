#pragma once

#include <cstddef>
#include <memory>

/** \brief A QuadNode encodes a single quadrant of the binary image.
 *
 * QuadNodes can be default constructed, then lazily initialized via QuadNode::init().
 * Alternatively, a QuadNode can be created and initialized by the specialized constructor.
 *
 * A QuadNode represents a quadrant of the image. Their size and color can be queried via
 * QuadNode::get_side_length() and QuadNode::get_color_value() respectively.
 *
 * Their offset within the image is encoded by the parent QuadTree structure. Clients will
 * typically interface with QuadNodes through the parent QuadTree, for which the public API are the
 * set of const member functions.
 *
 * A QuadNode will have 0 children if it is a leaf node, which encodes a homogenous quadrant.
 * Otherwise, it will have 4 children, which can be retrieved via QuadNode::get_children(). */
class QuadNode
{
public:
    /** \brief Each node can represent a Black, White, or Mixed-color quadrant. */
    enum class ColorValue {
        Mixed, ///< The quadrant contains both black and white pixels, ie. not a leaf
        Black,
        White
    };

    /** \brief Creates and initializes a node
     *
     * \param side_length The size of this quadrant's sides.
     * \param color The fill color for this quadrant, or Mixed if its not a leaf node. */
    QuadNode(size_t side_length, ColorValue color);

    /** \brief Creates an uninitialized node.
     *
     * Nodes are lazily initialized via init(). */
    QuadNode();

    /** \brief Initializes the Node, \sa QuadNode::QuadNode(size_t, ColorValue). */
    void init(size_t side_length, ColorValue color);

    /** \brief Query the size of the quadrant this node encodes.
     *
     * For non-leaf nodes, this is the size of the quadrant encoded by this node's children, \sa
     * is_leaf().
     *
     * \return The length of this quadrant's sides. */
    size_t get_side_length() const;

    /** \brief Query this quadrant's color.
     *
     * This has no actionable significance if the node is not a leaf, \sa is_leaf().
     *
     * \return Fill color for this quadrant. */
    ColorValue get_color_value() const;

    /** \brief Query if a terminal node.
     *
     * Undefined behavior if this node is not valid, \sa is_valid().
     *
     * \return true iff this is a valid, terminal node (has no children). */
    bool is_leaf() const;

    /** \brief Query whether Node is valid.
     *
     * In this context, valid means the node has been initialized, and has 0 or 4 children.
     * The validity of each child is not asserted, that responsibility is retained by the parent
     * tree structure.
     *
     * \return true iff this node is valid. */
    bool is_valid() const;

    /** \brief Defines a quad of values. */
    template<typename T>
    struct Quad {
        T q1; ///< Quadrant 1 value
        T q2; ///< Quadrant 2 value
        T q3; ///< Quadrant 3 value
        T q4; ///< Quadrant 4 value
    };

    /** \brief Sets or replaces the children of this node.
     *
     * By design, there is no child->parent relationship.
     *
     * This call will fail if any of the children are unset (null) or invalid, \sa is_valid(). In
     * this case, this Node's children will be null (re)initialized.
     *
     * By setting the children, the caller is transferring their ownership to node. Accepting
     * unique_ptr by copy reinforces this contract (see
     * <a href="https://herbsutter.com/2013/06/05/gotw-91-solution-smart-pointer-parameters/">
     *      Herb Sutter's opinion on Smart Pointer Parameters.
     * </a>)
     *
     * \param children The new children for this node.
     * \return true iff the call succeeded. */
    bool set_children(Quad<std::unique_ptr<QuadNode>> children);

    /** \brief Retrieve this node's children.
     *
     * The returned children will be null-initialized if this node has no children, \sa is_leaf().
     *
     * \return The children of this node. */
    Quad<std::shared_ptr<QuadNode>> get_children() const;

    /** \brief Equality comparison.
     *
     * Two nodes may be equivalent, but encode different data within the image, if their properties
     * are the same, but are located at different locations with the parent QuadTree. That is,
     * equality comparison for nodes is agnostic to its context within the parent QuadTree.
     *
     * Results are undefined if either node is invalid.
     *
     * \return true iff both nodes are valid and have the same properties. */
    bool operator==(const QuadNode& other) const;

    /** \brief \sa operator==() */
    bool operator!=(const QuadNode& other) const;

private:
    bool was_initialized_; ///< True iff node was initialized
    size_t side_length_;   ///< This Node's size, in pixels
    ColorValue color_;     ///< Pixel color for this node

    Quad<std::shared_ptr<QuadNode>> children_;    ///< Storage for this Node's Children

    /** \brief Query validity of the children, \sa is_valid().
     *
     * \return true iff all children references are initialized (not null). */
    bool has_valid_children() const;
};
