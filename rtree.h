#ifndef HEADER_FILE
#define HEADER_FILE

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

// typedef struct leaf* LEAF;

// struct leaf
// {
//     int count;
//     struct data entries[M];
// };

typedef struct node* NODE;

enum kind {
    LEAF = 1,
    BRANCH = 2,
};

struct node
{   enum kind kind;
    int count;
    struct rect rect[M];
    struct rect mbr;
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

struct data search(int x1, int x2, int y1, int y2){}
void insert(int x1, int y){}
void createTree(struct rtree *tree, NODE *leavesList, int count);
NODE* createNodes(NODE *leavesList, int size);
NODE createLeaf(struct data* data_entries, int count);
#endif
