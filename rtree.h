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

struct rect {
    int x_min;
    int y_min;
    int x_max;
    int y_max;
};


struct data search(int x1, int x2, int y1, int y2){}

void insert(int x1, int y){}

void createTree(struct rtree *tree, NODE *leavesList, int count);
NODE* createNodes(NODE *leavesList, int size);
NODE createLeaf(struct data* data_entries, int count);
#endif
