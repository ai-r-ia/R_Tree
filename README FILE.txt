README FILE

The file "DSA_assignment_group_28.c" contains the implmentation of an Rtree using the STR approach as mentioned in the reference paper: 
https://ieeexplore.ieee.org/stamp/stamp.jsp?tp=&arnumber=582015 

HOW TO RUN THE CODE
In order to run the .c file, please enter the following commands:
gcc -c DSA_assignment_group_28.c
gcc -o 28_rtree_exe DSA_assignment_group_28.o

After making the executable please input your file name when running the executable. It should be entered in the following format.
./28_rtree_exe <file_name>

If your input file is data.txt, please run it as shown below:
./28_rtree_exe data.txt

OUTPUT FORMAT EXPECTED
The code will form an R-Tree (by the STR method) using the input data given in the .txt file mentioned while running. 
It will print the tree using the prorder traversal in the following manner:

For every internal node, the following will be printed- 
"Internal node mbr:- bottom left:(_, _), top right:(_, _)"
where the blank spaces will be filled by the bottom left and top right coordinates of the minimum bounding rectangle of the internal node.

For every external (leaf) node, it will print-
"External node mbr:- bottom left:(_, _), top right:(_, _)
2D objects in Leaf(external) nodes: (_, _), (_, _) ...."

where the initial blank spaces will be filled by the bottom left and top right coordinates of the minimum bounding rectangle of the internal node and the blanks in the second line are the 2D data entires stored in that external (leaf) node.


