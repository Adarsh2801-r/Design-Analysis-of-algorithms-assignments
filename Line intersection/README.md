# Design-Analysis-of-algorithms-assignments

## Bentley-Ottman algorithm for reporting and counting line segment intersections

### Geometric Assumptions

  1.  At any intersection point, not more than 2 line segments intersect .

  2.  No line segment has an endpoint that is a part of some other line segment.

### Implementation details

**Data Structures**

1. eventQueue :   Maintains point ordered by the x-coordinate.  Implemented using Red-black tree (Balanced binary search                                           tree) which supports insert and search operations.

2. sweepLineStatus : Maintains line segments with y-coordinates sorted for a given x-coordinate value. Implemented using                                                   Red-black tree (Balanced binary  tree) which supports insert , search , delete and swap operations.

**Sketch of the algorithm**

   1. Traverse the eventQueue and process the points in order . (iterative inorder traversal)

   2. Process the events as encountered by sweep line under following 3 cases :

       2.1  Point is left endpoint of segment :  Insert current segment into sweepLineStatus. Current segment is neighbour of above                                                                                 and below segments. Check for intersection current segment with its neighbours and                                                                                 insert those intersection points into eventQueue structure.

       2.2 Point is right endpoint of segment: Check for intersection between above and below line segments and add these points                                                                                   to event Queue. Delete current segment from sweepLineStatus.

       2.3 Point is intersection point :               Report the pair of segments as intersecting. Swap their positions in                                                                                                                  sweepLineStatus. Check for intersections between upper segment and segment above                                                                                it and similarly lower segment and segment below it and add these points to                                                                                                 eventQueue.

    

### Overall time complexity

Efficient data structures like RB tree/ AVL tree take O(logN) for insert , search and delete operations. Thus the overall time complexity to implement line segment intersection algorithm is O((2N+I)logN) , where N = number of line segments ; I = number of intersection points .

### Sample outputs
![dig3](https://user-images.githubusercontent.com/66625110/160598508-e215ffb9-fe3c-4bc3-ab01-d4a447e9a9c5.png)
![dig6](https://user-images.githubusercontent.com/66625110/160598454-f7047f09-6439-4d2c-b338-b11368ff1f03.png)
![dig12](https://user-images.githubusercontent.com/66625110/160598486-4ceff203-3c45-4b82-bef5-4156e4a47e1a.png)




