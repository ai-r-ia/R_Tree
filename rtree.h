#ifndef RTREE
#define RTREE

#define M 4
#define m 2
#define dimension 2

typedef struct data* Data;
struct data
{
    int x;
    int y;
};

struct rect {
    int x_min;
    int y_min;
    int x_max;
    int y_max;
};

typedef struct node* NODE;

enum type {
    LEAF = 1,
    INTERNAL_NODE = 2,
};

struct node
{   enum type type;
    int count;
    struct rect rect[M];
    struct rect mbr;
    struct data center;
    int area;
    union{
        struct node* node_children[M];
        struct data entries[M];
    };
};


struct rtree
{  
    int count;
    NODE root;
    int height;
    struct rect* rect;
};


typedef struct heap* Heap;
struct heap {
    Data points;
    int size;
    int capacity;
    int depth;
};

Heap heap_create();
int parent(Heap h, int node, int coord);
int left_child(Heap h, int node, int coord);
int  right_child(Heap h, int node, int coord);
void max_heapify(Heap h, int index, int coord);
Heap build_max_heap(Heap h, int coord);
Heap heap_sort(Heap h, int coord);

NODE createNode(int index, NODE *leavesList, int size);
struct data search(int x1, int x2, int y1, int y2) {}
void insert(int x1, int y){}
void createTree(struct rtree *tree, NODE *leavesList, int count);
NODE* createLevel(NODE *leavesList, int size);
NODE createLeaf(struct data* data_entries, int count);
struct rect findMBR(NODE given_node);
NODE *algo_str(NODE *leaveslist, int size);
struct data findCenter(NODE given_node);
int findArea(struct rect mbr);
struct rtree *generateTree(NODE *leavesList, int count);
// void printTree(struct rtree *tree);
void order(NODE root, NODE *nodesList, int index);
void preorder(NODE root, int n);

#endif
