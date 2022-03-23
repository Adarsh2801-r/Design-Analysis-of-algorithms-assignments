# Design-Analysis-of-algorithms-assignments

## Line-intersection

**Shamos & Hoeys algorithm for intersection detection**

Geometric Assumptions

  1.  At any intersection point, not more than 2 line segments intersect .

  2.  No line segment has an endpoint that is a part of some other line segment.

Implementation details

Data Structures

1. eventQueue :   Maintains point ordered by the x-coordinate.  Implemented using Red-black tree (Balanced binary search                                           tree) which supports insert and search operations.

2. sweepLineStatus : Maintains line segments with y-coordinates sorted for a given x-coordinate value. Implemented using                                                   Red-black tree (Balanced binary  tree) which supports insert , search , delete and swap operations.

Sketch of the algorithm

   1. Traverse the eventQueue and process the points in order . (iterative inorder traversal)

   2. Process the events as encountered by sweep line under following 3 cases :

       2.1  Point is left endpoint of segment :  Insert current segment into sweepLineStatus. Current segment is neighbour of above                                                                                 and below segments. Check for intersection current segment with its neighbours and                                                                                 insert those intersection points into eventQueue structure.

       2.2 Point is right endpoint of segment: Check for intersection between above and below line segments and add these points                                                                                   to event Queue. Delete current segment from sweepLineStatus.

       2.3 Point is intersection point :               Report the pair of segments as intersecting. Swap their positions in                                                                                                                  sweepLineStatus. Check for intersections between upper segment and segment above                                                                                it and similarly lower segment and segment below it and add these points to                                                                                                 eventQueue.

    

Overall time complexity

Efficient data structures like RB tree/ AVL tree take O(logN) for insert , search and delete operations. Thus the overall time complexity to implement line segment intersection algorithm is O((2N+I)logN) , where N = number of line segments ; I = number of intersection points .
