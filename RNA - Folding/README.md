# RNA FOLDING USING DP
## Base Pairing Rules
  1. (No sharp turns) The ends of each pair are separated by at least 4 intervening bases i.e. if (i , j) ε S, then i < j - 4.

 2. The elements in each pair in S consist of either {A,U} or {C,G} (in either order). 

 3. S is a matching: no base appears in more than one pair.

4. (No knots) If (i,j) and (k,l) are two pairs in S, then we cannot have i < k < j < l .

##Implementation details

**Approach**

1. OPT(i,j) is the maximum number of base pairs in a secondary structure for bibi+1 … bj. OPT(i,j) = 0, if i≥j-4.

2. In the optimal secondary structure on bibi+1 …bj 

   2.1 if j is not a member of any pair, use OPT(i,j-1).

   2.2 if j pairs with some t < j - 4, knot condition yields two independent sub-problems! OPT(i,t -1) and OPT(t+1,j-1)

**Recurrence Relation**

   
![image](https://user-images.githubusercontent.com/66625110/166203475-1a4b46e2-a7c7-431c-8654-2742a1b4da09.png)

    
## Overall time complexity

There are O(n^2) subproblems. Each subproblem requires linear pass between i to j-4. So overall time complexity is O(n^3).


