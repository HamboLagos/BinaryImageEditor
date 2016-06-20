# Binary Image Editor
Simple library for rotating and scaling binary images.

Problem statement borrowed from Drew Neil, content creator at
[Peer to Peer](http://peertopeer.io/videos/7-paul-battley/). See [Problem Description](problem_statement.pdf) for more
information.

## QuadTree Representation
In order to apply scale and rotate operations to our image, we need an appropriate data format. A QuadTrees will fit the
bill, but it comes with a few limitations.
For the non-degenerate case, our tree depth will be at least 1, so we should formalize it's structure. We can intuit
that each node can have either 0 children (as in the degenerate case) or 4 children (it's a **Quad**Tree, after all).

Let's walk through encoding a small image by hand, to help inform the algorithm we'll be developing.

### Example: 4x4 Heterogenous Image
#### Description
Given Binary Image (with "w" == white pixel, and  "b" == black pixel), sketch the quadtree representation, and show how
each node in the tree maps to a region of the image.

```
w w w b
w w w b
w b b b
b w b b
```

#### Solution
** Pseduo Code Algorithm **
```
function Encode (parameters: Image)
    create TreeRoot (with: Image.data)
    call EncodeRecursive (with: TreeRoot)

function EncodeRecursive (parameters: Root)
    if DegenerateCase?
        return
    else
        partition Root.data into 4 quads
        for each quad
            call EncodeRecursive (with: quad)
```

It was natural to define our pseudo-algorithm here recursively, but there are technical implications to consider when
actually implementing it that way. We'll come back to those.

We start by calling `Encode(Image)`, which creates the tree root, and starts the recursive solution. After the first
pass, our image and tree structure will look something like this

```
    root
     |
----------
|  |  |  |
n0 n1 n2 n3

q2       q1
  w w|w b
  w w|w b
  ---|---
  w b|b b
  b w|b b
q3       q4
```

Here, we need to specify the ordering of the mapping function from quadtree to image. Let's go with Cartesian
Coordinate, since it's easy to remember. Then, let's map nodes to quadrant in order:

```
{
  n0 => q1,
  n1 => q2,
  n2 => q3,
  n3 => q4
}
```

Next, we'd recurse, with n1 as the new root. The subtree and corresponding quadrants of the image for this level can be
deduced by inspection

```
     n0
     |
----------
|  |  |  |
n4 n5 n6 n7

q2   q1
  w|b
  -|-
  w|b
q3   q4
```

We apply the same logic to map each child node's position in the tree to quadrants of the image. Each quadrant is now
a single pixel, so we've hit the degenerate.

We move on the next sibling of n0, which is n1. We can see from the original 4x4 image that n1 is the
degenerate case, so it is the last node in its branch.

It's trivial to finish this example, but continuing won't lead to any further insight, so let's move on.

## Description of Data Structures
Let's formalize what we have so far

**QuadTree**
Root: is a `QuadNode`

**QuadNode**
Size: quadrant side length
Value: black or white
Children: 0 or 4 `QuadNode`

## Potential Improvements
We can find potential improvements to this library by extending it to remove
some of our earlier assumptions, such as

* Working with colored (non-binary) images
* Handling rectangular (non-square) images

Other limitations imposed by our chosen implementation

* Handling different image file types (.jpeg, .png, .bmp, etc)
