#pragma once

#include <cstddef>

/* class QuadTree */
/* { */
/* public: */
/*     /** \brief Constructs the quadtree representation of a binary image from its raw pixel data. */
/*      * */
/*      * Elements from the pixels array are black if set, and white if cleared. On success, the root of the tree can be */
/*      * retrieved with root(). If there's an error during construction, root() will return nullptr. */
/*      * */
/*      * \param pixels Flattened array of pixel values */
/*      * \param side_length Row and column size of image in pixels (square images only) *1/ */
/*     QuadTree(std::vector<bool> pixels, size_t side_length); */

/*     /// TODO ... scale(...) */
/*     /// TODO ... rotate(...) */

/* private: */
/*     /// TODO ... add_node(...) */
/* } */

class QuadNode
{
friend class QuadTree;
friend class TestableQuadNode;

public:
    /** \brief Nodes of this type can represent either white or black quadrants of the source image. */
    enum class ColorValue { Black, White };

    /** \brief Returns the length of this quad's sides. */
    size_t get_side_length() const;

    /** \brief Returns the color associated with this node.
     *
     * This has no meaning if the node is a leaf, and will return ColorValue::Black by default. Clients can check if
     * this node is a leaf by calling is_leaf()*/
    ColorValue get_color_value() const;

    /** \brief Returns true iff this is a leaf node (has no children), otherwise false. */
    bool is_leaf() const;

    /** \brief Returns true iff this node is valid (has been initialized). */
    bool is_valid() const;

protected:
    /** \brief Creates a new uninitialized quadnode.
     *
     * Uninitialized QuadNodes should be configured via init() before inseterting into tree. */
    QuadNode();

    /** \brief Creates a new QuadNode ready to be inserted into a QuadTree.
     *
     * \param side_length The length of this quadrant's sides in pixels
     * \param color This quadrant's color, meaningless if this is not a leaf node */
    QuadNode(size_t side_length, ColorValue color = ColorValue::Black);

    /** \brief Initializes this node, as if it had been constructed by the non-default constructor.
     *
     * An initialized Node is ready to be insterted into the tree. Clients can query this node's status via is_valid() */
    void init();

    /** \brief Quadrants are split following Cartesian Coordinate conventions.
     *
     * See the README for more details */
    struct Children {
        const QuadNode& q1; ///< quadrant 1 child
        const QuadNode& q2; ///< quadrant 2 child
        const QuadNode& q3; ///< quadrant 3 child
        const QuadNode& q4; ///< quadrant 4 child
    };

    /** \brief Adds the given nodes as children of this node. */
    void add_children(const Children& children);

private:
    size_t side_length;
};
