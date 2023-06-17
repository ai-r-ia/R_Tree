#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define min(a,b) (((a) < (b)) ? (a) : (b))
#define max(a,b) (((a) > (b)) ? (a) : (b))
#define b 4
#define dimension 2

//defining a struct to store data points given as input
typedef struct data *Data;
struct data
{
    int x;
    int y;
};

//defining a struct to store centers of minimum bounding rectangles(MBRs)
typedef struct center *Center;
struct center
{
    double x;
    double y;
};

// defining a struct to store the bottom left and top right coordinates of a minimum bounding rectangle(MBR)
struct box
{
    int x_min;
    int y_min;
    int x_max;
    int y_max;
};

//defining a struct to store nodes
typedef struct node *NODE;
//type defined to distinguish between internal and external(leaf) nodes
enum type
{
    LEAF = 1,
    INTERNAL_NODE = 2,
};

struct node
{
    enum type type;
    int count;
    struct box box[b];
    struct box mbr;
    struct center center;
    int area;
    union
    {
        struct node *node_children[b];
        struct data entries[b];
    };
};

//defining a struct to store the tree
struct rtree
{
    int count;
    NODE root;
    int height;
    struct box *box;
};

//defining a heap for applying heapsort on the data input given
typedef struct heap *Heap;
struct heap
{
    Data points;
    int size;
    int capacity;
    int depth;
};

// defining a heap for applying heapsort on the nodes at each level
typedef struct heapNode *HeapNode;
struct heapNode
{
    NODE *points;
    int size;
    int capacity;
    int depth;
};

//function declarations
int _ceil(double x);
int _floor(double x);
double _sqrt(int x);

Heap heap_create();
int parent(Heap h, int node, int coord);
int left_child(Heap h, int node, int coord);
int right_child(Heap h, int node, int coord);
HeapNode heap_create_node();
int parent_node(HeapNode h, int node, int coord);
int left_child_node(HeapNode h, int node, int coord);
int right_child_node(HeapNode h, int node, int coord);
void max_heapify(Heap h, int index, int coord);
Heap build_max_heap(Heap h, int coord, int size);
Heap heap_sort(Heap h, int coord, int size);
void max_heapify_nodes(HeapNode h, int index, int coord);
HeapNode build_max_heap_nodes(HeapNode h, int coord, int size);
HeapNode heap_sort_nodes(HeapNode h, int coord, int size);

NODE createLeaf(struct data *data_entries, int count);
NODE createNode(int index, NODE *leavesList, int size);
NODE *createLevel(NODE *leavesList, int size);
struct rtree *generateTree(NODE *leavesList, int count);
void createTree(struct rtree *tree, NODE *leavesList, int count);
int findArea(struct box mbr);
struct center findCenter(NODE given_node);
struct box findMBR(NODE given_node);
struct data search(int x1, int x2, int y1, int y2);
void preorder(NODE root,  int index);
Data dataSorting(Data data_entries, int data_size, int S);
NODE *sorting(NODE *nodesList, int size);

//returns the ceil of a double input given
int _ceil(double x){
    int x_int = (int)x;
    if((double)x_int == x){
        return x_int;
    }
    return x_int +1;
}

//returns the floor of a double input given
int _floor(double x)
{
    int x_int = (int)x;
    if ((double)x_int == x || x_int > 0)
    {
        return x_int;
    }
    return x_int - 1;
}

//returns the square root of an int input given
double _sqrt(int x){
    int first = 0, last = x;
    int mid;
    double res;

    while (first <= last)
    {
        mid = (first + last) / 2;

        if ((mid * mid) == x)
        {
            res = mid;
            break;
        }
        if ((mid * mid )> x)
        {
            last = mid - 1;
        }
        else
        {
            res = first;
            first = mid + 1;
        }
    }

    double increment = 0.1;
    for (int i = 0; i < 6; i++)
    {
        while (res * res <= x)
        {
            res += increment;
        }
        res = res - increment;
        increment = increment / 10;
    }
    return res;
}

//constructor for the heap struct
Heap heap_create()
{
    Heap h = malloc(sizeof(struct heap));
    h->size = 0;
    h->capacity = 1;
    h->depth = 0;
    return h;
}

// finds parent of a given node in the heap
int parent(Heap h, int node, int coord)
{
    int parent;
    parent = _floor(node / 2);
    if (parent >= 0)
    {
        return parent;
    }
    else
    {
        return node;
    }
}

//finds left child of a given node in the heap
int left_child(Heap h, int node, int coord)
{
    int left;
    left = 2 * (node) + 1;
    if (left < h->size)
    {
        return left;
    }
    else
    {
        return node;
    }
}

// finds right child of a given node in the heap
int right_child(Heap h, int node, int coord)
{
    int right;
    right = 2 * (node) + 2;
    if (right < h->size)
    {
        return right;
    }
    else
    {
        return node;
    }
}

//constructor for the heapNode struct
HeapNode heap_create_node()
{
    HeapNode h = malloc(sizeof(struct heapNode));
    h->size = 0;
    h->capacity = 1;
    h->depth = 0;
    return h;
}

// finds parent of a given node in the heapNode
int parent_node(HeapNode h, int node, int coord)
{
    int parent;
    parent = _floor(node / 2);
    if (parent >= 0)
    {
        return parent;
    }
    else
    {
        return node;
    }
}

// finds left child of a given node in the heapNode
int left_child_node(HeapNode h, int node, int coord)
{
    int left;
    left = 2 * (node) + 1;
    if (left < h->size)
    {
        return left;
    }
    else
    {
        return node;
    }
}

// finds right child of a given node in the heapNode
int right_child_node(HeapNode h, int node, int coord)
{
    int right;
    right = 2 * (node) + 2;
    if (right < h->size)
    {
        return right;
    }
    else
    {
        return node;
    }
}

//implementation of heapify in heapsort for struct heap
void max_heapify(Heap h, int index, int coord)
{
    int left = left_child(h, index, coord);
    int right = right_child(h, index, coord);
    int largest = index;
    if (coord == 0)
    {
        if (left < h->size && h->points[left].x > h->points[largest].x)
        {
            largest = left;
        }
        if (right < h->size && h->points[right].x > h->points[largest].x)
        {
            largest = right;
        }
    }
    else if (coord == 1)
    {
        if (left < h->size && h->points[left].y > h->points[largest].y)
        {
            largest = left;
        }
        if (right < h->size && h->points[right].y > h->points[largest].y)
        {
            largest = right;
        }
    }

    if (largest != index)
    {
        struct data temp = h->points[index];
        h->points[index] = h->points[largest];
        h->points[largest] = temp;
        max_heapify(h, largest, coord);
    }
}

// buillds a max heap for struct heap
Heap build_max_heap(Heap h, int coord, int size)
{
    h->size = size;
    for (int i = _floor(h->size / 2); i >= 0; i--)
    {
        max_heapify(h, i, coord);
    }
    return h;
}

// applies heapsort on the given heap
Heap heap_sort(Heap h, int coord, int size)
{
    h = build_max_heap(h, coord, size);
    for (int i = h->size - 1; i >= 1; i--)
    {
        struct data temp = h->points[0];
        h->points[0] = h->points[i];
        h->points[i] = temp;
        h->size = h->size - 1;
        max_heapify(h, 0, coord);
    }
    return h;
}

// implementation of heapify in heapsort for struct heapNode
void max_heapify_nodes(HeapNode h, int index, int coord)
{
    int left = left_child_node(h, index, coord);
    int right = right_child_node(h, index, coord);
    int largest = index;
    if (coord == 0)
    {
        if (left < h->size && h->points[left]->center.x > h->points[largest]->center.x)
        {
            largest = left;
        }
        if (right < h->size && h->points[right]->center.x > h->points[largest]->center.x)
        {
            largest = right;
        }
    }
    else if (coord == 1)
    {
        if (left < h->size && h->points[left]->center.y > h->points[largest]->center.y)
        {
            largest = left;
        }
        if (right < h->size && h->points[right]->center.y > h->points[largest]->center.y)
        {
            largest = right;
        }
    }

    if (largest != index)
    {
        struct node *temp = h->points[index];
        h->points[index] = h->points[largest];
        h->points[largest] = temp;
        max_heapify_nodes(h, largest, coord);
    }
}

// buillds a max heap for struct heapNode
HeapNode build_max_heap_nodes(HeapNode h, int coord, int size)
{
    h->size = size;
    for (int i = _floor(h->size / 2); i >= 0; i--)
    {
        max_heapify_nodes(h, i, coord);
    }
    return h;
}

// applies heapsort on the given heapNode
HeapNode heap_sort_nodes(HeapNode h, int coord, int size)
{
    h = build_max_heap_nodes(h, coord, size);
    for (int i = h->size - 1; i >= 1; i--)
    {
        struct node *temp = h->points[0];
        h->points[0] = h->points[i];
        h->points[i] = temp;
        h->size = h->size - 1;
        max_heapify_nodes(h, 0, coord);
    }
    return h;
}

// constructor to create a leaf(external) node
NODE createLeaf(struct data* data_entries, int count){

    NODE newLeaf;
    newLeaf = malloc(sizeof(struct node));
    newLeaf->count=0;
    newLeaf->type = 1;

    for(int i=0; i<count;i++){
        newLeaf->entries[i] = data_entries[i];
        newLeaf->box[i] = (struct box){.x_min = data_entries[i].x, .y_min = data_entries[i].y,
         .x_max = data_entries[i].x, .y_max = data_entries[i].y};
        newLeaf->count++;
    }

    newLeaf->mbr = findMBR(newLeaf);
    newLeaf->center = findCenter(newLeaf);
    newLeaf->area = findArea(newLeaf->mbr);
    return newLeaf;
}

//constructor to create an internal node
NODE createNode(int index,  NODE *leavesList , int size)
{
    NODE newInternalNode;
    newInternalNode = malloc(sizeof(struct node));
    newInternalNode->count = 0;
    newInternalNode->type = 2;

    for(int i = 0; i< b && index<size; i++){
        newInternalNode->node_children[i] = leavesList[index];
        newInternalNode->box[i] = (struct box){.x_min = leavesList[index]->mbr.x_min,
        .y_min = leavesList[index]->mbr.y_min, .x_max = leavesList[index]->mbr.x_max, 
        .y_max = leavesList[index]->mbr.y_max};
        index++;
        newInternalNode->count++;
    }

    newInternalNode->mbr = findMBR(newInternalNode);
    newInternalNode->center = findCenter(newInternalNode);
    newInternalNode->area = findArea(newInternalNode->mbr);

    return newInternalNode;
}

//creates one level of the tree at a time
NODE* createLevel(NODE *leavesList, int size){

    int no_of_nodes = _ceil(size/(b*1.0));
    NODE *nodesList = malloc(no_of_nodes*sizeof(NODE));
    int index=0;
    for(int i =0; i<no_of_nodes; i++){
        nodesList[i] = createNode(index, leavesList, size);
        index += b;
    }
    nodesList = sorting(nodesList, no_of_nodes);
    return nodesList ;
}

//generates an Rtree
struct rtree *generateTree(NODE *leavesList, int count)
{
    struct rtree *tree = malloc(sizeof(struct rtree));
    // tree->root = malloc(sizeof(struct node));
    tree->count=count +1;
    tree->height=1;
    createTree(tree, leavesList, count);
    preorder(tree->root,tree->root->count);
}

//helps generateTree to create the Rtree
void createTree(struct rtree *tree, NODE *leavesList, int count)
{
    int no_of_nodes = _ceil(count / (b* 1.0));
    if (no_of_nodes == 1)
    {  
        tree->root = createNode(0, leavesList, count);
        tree->height++;
    }
    else
    {
        printf("creating nodes....\n");
        NODE *nodesList = malloc(sizeof(NODE) * no_of_nodes);
        nodesList = createLevel(leavesList, count);
        tree->height++;
        tree->count += no_of_nodes;
        createTree(tree, nodesList, no_of_nodes);
    }
}

// to find the area of the minimum bounding rectangle(mbr) for a given node
int findArea(struct box mbr)
{
        return (mbr.x_max - mbr.x_min) * (mbr.y_max - mbr.y_min);
}

// to find the center of the minimum bounding rectangle(mbr) for a given node
struct center findCenter(NODE given_node)
{
        struct center coord;
        coord.x = (given_node->mbr.x_min + given_node->mbr.x_max) / 2.0;
        coord.y = (given_node->mbr.y_min + given_node->mbr.y_max) / 2.0;
        return coord;
}

//to find bottom left and top right coordinates of minimum bounding rectangles (mbr)
struct box findMBR(NODE given_node)
{
    struct box mbr = given_node->box[0];
    for (int i = 1; i < given_node->count; i++)
    {
        if(given_node->type == 1){
            mbr.x_min = min(mbr.x_min, given_node->entries[i].x);
            mbr.y_min = min(mbr.y_min, given_node->entries[i].y);
            mbr.x_max = max(mbr.x_max, given_node->entries[i].x);
            mbr.y_max = max(mbr.y_max, given_node->entries[i].y);
        }else{
            mbr.x_min = min(mbr.x_min, given_node->box[i].x_min);
            mbr.y_min = min(mbr.y_min, given_node->box[i].y_min);
            mbr.x_max = max(mbr.x_max, given_node->box[i].x_max);
            mbr.y_max = max(mbr.y_max, given_node->box[i].y_max);
        }
    }
    return mbr;
}

// preorder traversal of the Rtree
void preorder(NODE root, int index)
{
    if (root->type == 1)
    {
        printf("External node mbr:- bottom left:(%d, %d), top right:(%d, %d)\n",
         root->mbr.x_min, root->mbr.y_min, root->mbr.x_max, root->mbr.y_max);
            printf("2D objects in Leaf(external) nodes: "); 
        for (int j = 0; j < root->count; j++)
        {
            printf("(%d, %d) ",root->entries[j].x, root->entries[j].y);
        }
        printf("\n");
    }
    else if (root->type == 2){
        printf("Internal node mbr:- bottom left:(%d, %d), top right:(%d, %d)\n",
         root->mbr.x_min, root->mbr.y_min, root->mbr.x_max, root->mbr.y_max);
        for(int i = 0; i<root->count; i++){
            preorder(root->node_children[i],root->node_children[i]->count);
        }
    }
}

// sorting data list based on STR implementation of Rtrees
Data dataSorting (Data data_entries, int data_size, int S){

    // applying heapsort to sort based on x and y
    Heap h = heap_create();
    h->points = data_entries;

    h = build_max_heap(h, 0, data_size); // building max heap
    h = heap_sort(h, 0, data_size);      // sorting based on x

    for (int i = 0; i < data_size; i++)
    {
        data_entries[i] = h->points[i];
    }

    int remaining = data_size;
    for (int k = 0; k < S; k++)
    {
        if (remaining <= 0)
        {
            break;
        }
        int ele = min(S * b, remaining);

        Heap ySortingHeap = heap_create();
        ySortingHeap->points = malloc(sizeof(struct data) * ele);

        for (int i = k * S * b, l = 0; i < min((k + 1) * S * b, data_size) - k, l < ele; i++, l++)
        {
            ySortingHeap->points[l] = data_entries[i];
        }

        ySortingHeap = build_max_heap(ySortingHeap, 1, ele); // building max heap
        ySortingHeap = heap_sort(ySortingHeap, 1, ele);      // sorting based on y

        for (int i = k * S * b, l = 0; i < min((k + 1) * S * b, data_size) - k, l < ele; i++, l++)
        {
            data_entries[i] = ySortingHeap->points[l];
        }

        free(ySortingHeap->points);
        free(ySortingHeap);
        remaining = remaining - (S * b);
    }
    return data_entries;
}

//sorting nodes list based on STR implementation of Rtrees
NODE* sorting(NODE* nodesList, int size)
{
    HeapNode h = heap_create_node();
    h->points = nodesList;

    int P = _ceil(size / (double)b);
    int S = _ceil(_sqrt(P));

    h = build_max_heap_nodes(h, 0, size); // building max heap
    h = heap_sort_nodes(h, 0, size);      // sorting based on x

    for (int i = 0; i < size; i++)
    {
        nodesList[i] = h->points[i];
    }

    int remaining = size;
    for (int k = 0; k < S; k++)
    {
        if (remaining <= 0)
        {
            break;
        }
        int ele = min(S * b, remaining);

        HeapNode ySortingHeap = heap_create_node();
        ySortingHeap->points = malloc(sizeof(struct node) * ele);

        for (int i = k * S * b, l = 0; i < min((k + 1) * S * b, size) - k, l < ele; i++, l++)
        {
            ySortingHeap->points[l] = nodesList[i];
        }

        ySortingHeap = build_max_heap_nodes(ySortingHeap, 1, ele); // building max heap
        ySortingHeap = heap_sort_nodes(ySortingHeap, 1, ele); // sorting based on y

        for (int i = k * S * b, l = 0; i < min((k + 1) * S * b, size) - k, l < ele; i++, l++)
        {
            nodesList[i] = ySortingHeap->points[l];
        }

        free(ySortingHeap->points);
        free(ySortingHeap);
        remaining = remaining - (S * b);
    }
    return nodesList;
}

//Driver code
int main(int argc, char *argv[])
{
    int data_size, P, S;

    // reading the data
    FILE *fp;

    if(argc != 2){
        printf("Please enter filename in proper format.");
        return 1;
    }
    fp = fopen(argv[1], "r");
    if (fp == NULL)
    {
        printf("Error opening file");
        exit(1);
    }
    int lineCounter = 0;
    while (!feof(fp))
    {
        int x, y;
        fscanf(fp, "%d %d", &x, &y);
        lineCounter++;
    }
    fclose(fp);

    data_size = lineCounter;
    Data data_entries = malloc(sizeof(struct data)* data_size);
    
    P = _ceil(data_size / (double)b);
    S = _ceil(_sqrt(P));

    fp = fopen(argv[1], "r");
    int i = 0;
    while (!feof(fp))
    {
        int x, y;
        fscanf(fp, "%d %d", &data_entries[i].x, &data_entries[i].y);
        i++;
    }
    fclose(fp);

    // implementing str at leaf level
    data_entries = dataSorting(data_entries, data_size, S);

    // creating list of leaves
    NODE *leaves_list = malloc(sizeof(NODE) * P);
    int r = data_size;
    for(i = 0 ; i<P; i++){
        leaves_list[i] = createLeaf(data_entries+i*b,r>b?b:r);
        r=r-b;
    }
 
     leaves_list = sorting(leaves_list, P);
    
    // creating tree
    generateTree(leaves_list, P);

}