#pragma once

#include <memory>
#include <vector>

#include "quad_node.hpp"

/** \brief Encodes a binary image as a quadtree, \ref README.md.
 *
 * On Construction, the QuadTree is empty, and is initialized from the binary image data via init().
 * Clients can check whether initialization was successful via QuadTree::is_valid(). */
class QuadTree
{
friend class TestableQuadTree;

template<typename T> using Quad = QuadNode::Quad<T>;
using Data = std::vector<QuadNode::ColorValue>;
using Rows = std::vector<Data>;

public:
    /** \brief Constructs an unitialized QuadTree.
     *
     * QuadTrees are lazily initialized via init(). */
    QuadTree();

    /** \brief Initializes this QuadTree.
     *
     * Data is a contiguous array of the scanned pixels. Pixel ordering is irrelavent (top to
     * bottom, bottom to top, etc) so long as the scanning is performed in the same order when
     * reparsing the image from the tree.
     *
     * Only square images (height == width) are accepted. The size of the image is determined from
     * the size of the passed pixel data.
     *
     * \param data Binary image data. */
    void init(const Data& data);

    /** \brief Query validity of this tree.
     *
     * In this context, a tree is valid iff it has been initialized successfully, and all its nodes
     * are valid. \sa QuadNode::is_valid().
     *
     * \return true iff this is a valid tree. */
    bool is_valid() const;

    /** \brief Equality comparison.
     *
     * Equality in this context means that both trees encode the same data, ie. the images created
     * by parsing each tree are identical.
     *
     * Results are undefined if either tree is invalid.
     *
     * \return true iff both trees are valid and encode the same image. */
    bool operator==(const QuadTree& other) const;

    /** \brief \sa operator==(). */
    bool operator!=(const QuadTree& other) const;

    /// TODO define interface for applying operations

private:
    std::shared_ptr<QuadNode> root_; // This tree's root node

    /** \brief Initialize a QuadTree from an existing node.
     *
     * Performs no processing, but accepts the given node as the root of a QuadTree. Useful for
     * viewing subtrees within a parent tree.
     *
     * Clients should still check the validity of the tree before using it, \sa is_valid();
     *
     * \param root The initialized root of the tree. */
    QuadTree(std::shared_ptr<QuadNode> root);

    /** \brief Scans the input data, and returns it formatted as a 2D matrix of rows.
     *
     * Example:
     * \verbatim
     * Data = { Black, Black
     *          White, White }
     *
     * Rows = { {Black, Black},
     *          {White, White} }
     * \endverbatim
     *
     * Since we only deal with square images, the length of each row will always equal the number of
     * rows parsed, and is trivial to calculate from the input data size.
     *
     * \param data The pixel data to parse.
     * \return input data formatted to rows.
     * */
    static Rows parse_rows(const Data& data);

    /** \brief Splits the input data into its 4 quadrants.
     *
     * Quadrants are return in Cartesian Ordering (ie. NE, NW, SW, SE);
     *
     * Example:
     * \verbatim
     * Rows = { {White, Black},
     *          {White, Black} }
     * Quad = { {Black}, {White}, {White}, {Black} }
     * \endverbatim
     *
     * \param rows The rows to parse.
     * \return The 4 quadrants of the input data. */
    static Quad<Rows> get_quadrants(const Rows& rows);

    /** \brief Returns a copy of the data in the quadrant at the given offset.
     *
     * Example:
     * \verbatim
     * Rows = { {White, Black},
     *          {Black, White} }
     *
     * parse_quadrant(Rows, 0, 0) == {White}
     * parse_quadrant(Rows, 1, 0) == {Black}
     * parse_quadrant(Rows, 0, 1) == {Black}
     * parse_quadrant(Rows, 1, 1) == {White}
     * \endverbatim
     *
     * x_off, y_off are static offsets used to find the beginning of the quadrant to copy. The
     * copied quadrant's size is calculated from the size of the input data.
     *
     * \param rows The input data to copy from
     * \param x_off x_offset for the quadrant to copy
     * \param y_off y_offset for the quadrant to copy
     * \return A copy of the data in the given quadrant. */
    static Rows parse_quadrant(const Rows& rows, size_t x_off, size_t y_off);

    /** \brief Builds a Quad of Nodes which encode the given pixel data.
     *
     * \param quadrants The pixel data for each node.
     * \return A Quad of Nodes which encode the input pixel data. */
    static Quad<std::unique_ptr<QuadNode>> make_nodes(const Quad<Rows>& quadrants);

    /** \brief Constructs a new node which encodes the given pixel data.
     *
     * \param rows The pixel data to encode.
     * \return A Node which encodes the given pixel data. */
    static std::unique_ptr<QuadNode> make_node(const Rows& rows);

    /** \brief Recursively initializes each subtree.
     *
     * Any heterogenous image of size > 1 pixel will require a tree of minimum depth 1 to encode it.
     * Any given Node in the tree maintains the "Quad-Tree-Ness" of its parent, which lends itself
     * to defining this initialization recursively.
     *
     * See the wikipedia page on <a href="https://en.wikipedia.org/wiki/Quadtree">QuadTrees</a>.
     *
     * \param rows The pixel data to be encoded.
     * \param parent The parent node the created subtree should be attached to. */
    void init_recursive(const Rows& rows, QuadNode& parent);
};
