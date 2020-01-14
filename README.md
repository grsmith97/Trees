# Trees
Implements several Binary Tree structures in C

Object oriented programming is more difficult in the C language than it would be in modern languages such as Java or C#,
but object oriented functionality can be achieved using pointers (a lot of them).

The TNODE module is a wrapper module to be implemented in the tree structures. It uses void pointers which can use any data type
which could be referenced using the pointers. The String.c module is the example I most commonly used for testing.

The BST module is a standard binary search tree. It implements several methods typical in binary tree operations such as Insert,
Delete, and Search. The methods typically implement recursion to traverse the tree.

The GST module is a specialized BST called a Green Search Tree. I implemented a pseudo-inheritance functionality by including a 
BST pointer in the GST module. The main difference in functionality is that the Green Tree Nodes have a counter for duplicates,
meaning that if a duplicate node was to be inserted (or deleted) from the tree, the duplicate count for the GST node would be modified,
rather than a new node being created and inserted like in the BST.

The RBT module is a Red/Black self balancing tree. The explanation of the algorithms for how it achieves self-balancing functionality
is long winded and I won't bore the reader with the full explanation here, but it essentially checks differences in height between
subtrees when inserting and deleting and performs rotations to maintain a nearly complete/full tree structure. The purpose being
an increase in runtime efficiency in some cases. Details on the algorithms I used can be found here: 
https://www.geeksforgeeks.org/red-black-tree-set-1-introduction-2/

Overall, this project was probably the most difficult and the most time-consuming project I had done to date, considering the development
for this project occured fairly early in my programming career.
