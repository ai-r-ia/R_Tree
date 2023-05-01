#include<math.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
// #include "rtree.h"
// #include "heapsort.c"

#define min(a,b) (((a) < (b)) ? (a) : (b))
#define max(a,b) (((a) > (b)) ? (a) : (b))
#define M 4
#define m 2
#define dimension 2

typedef struct data *Data;
struct data
{
    int x;
    int y;
};

typedef struct center *Center;
struct center
{
    double x;
    double y;
};

struct rect
{
    int x_min;
    int y_min;
    int x_max;
    int y_max;
};

typedef struct node *NODE;

enum type
{
    LEAF = 1,
    INTERNAL_NODE = 2,
};

struct node
{
    enum type type;
    int count;
    struct rect rect[M];
    struct rect mbr;
    struct center center;
    int area;
    union
    {
        struct node *node_children[M];
        struct data entries[M];
    };
};

struct rtree
{
    int count;
    NODE root;
    int height;
    struct rect *rect;
};

typedef struct heap *Heap;
struct heap
{
    Data points;
    int size;
    int capacity;
    int depth;
};

typedef struct heapNode *HeapNode;
struct heapNode
{
    NODE *points;
    int size;
    int capacity;
    int depth;
};

Heap heap_create();
int parent(Heap h, int node, int coord);
int left_child(Heap h, int node, int coord);
int right_child(Heap h, int node, int coord);
void max_heapify(Heap h, int index, int coord);
Heap build_max_heap(Heap h, int coord, int size);
Heap heap_sort(Heap h, int coord, int size);

NODE createNode(int index, NODE *leavesList, int size);
struct data search(int x1, int x2, int y1, int y2) {}
void insert(int x1, int y) {}
void createTree(struct rtree *tree, NODE *leavesList, int count);
NODE *createLevel(NODE *leavesList, int size);
NODE createLeaf(struct data *data_entries, int count);
struct rect findMBR(NODE given_node);
NODE *algo_str(NODE *leaveslist, int size);
struct center findCenter(NODE given_node);
int findArea(struct rect mbr);
struct rtree *generateTree(NODE *leavesList, int count);
// void printTree(struct rtree *tree);
void preorder(NODE root,  int index);
NODE *sorting(NODE *nodesList, int size);

Heap heap_create()
{
    Heap h = malloc(sizeof(struct heap));
    // h->points = malloc(sizeof(struct data));
    h->size = 0;
    h->capacity = 1;
    h->depth = 0;
    return h;
}

HeapNode heap_create_node()
{
    HeapNode h = malloc(sizeof(struct heapNode));
    // h->points = malloc(sizeof(struct node));
    h->size = 0;
    h->capacity = 1;
    h->depth = 0;
    return h;
}

int parent(Heap h, int node, int coord)
{
    int parent;
    parent = floor(node / 2);
    if (parent >= 0)
    {
        return parent;
    }
    else
    {
        return node;
    }
}

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

int parent_node(HeapNode h, int node, int coord)
{
    int parent;
    parent = floor(node / 2);
    if (parent >= 0)
    {
        return parent;
    }
    else
    {
        return node;
    }
}

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

Heap build_max_heap(Heap h, int coord, int size)
{
    h->size = size;
    for (int i = floor(h->size / 2); i >= 0; i--)
    {
        max_heapify(h, i, coord);
    }
    return h;
}

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

void max_heapify_nodes(HeapNode h, int index, int coord)
{
    int left = left_child_node(h, index, coord);
    int right = right_child_node(h, index, coord);
    int largest = index;
    if (coord == 0)
    {
        // printf("%f", h->points[left]->center.x);
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

HeapNode build_max_heap_nodes(HeapNode h, int coord, int size)
{
    h->size = size;
    for (int i = floor(h->size / 2); i >= 0; i--)
    {
        max_heapify_nodes(h, i, coord);
    }
    return h;
}

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

NODE createLeaf(struct data* data_entries, int count){

    NODE newLeaf;
    newLeaf = malloc(sizeof(struct node));
    newLeaf->count=0;
    newLeaf->type = 1;

    for(int i=0; i<count;i++){
        newLeaf->entries[i] = data_entries[i];
        newLeaf->rect[i] = (struct rect){.x_min = data_entries[i].x, .y_min = data_entries[i].y, .x_max = data_entries[i].x, .y_max = data_entries[i].y};
        newLeaf->count++;
    }

    newLeaf->mbr = findMBR(newLeaf);
    printf("found mbr: %d, %d, %d, %d...leaf\n", newLeaf->mbr.x_min, newLeaf->mbr.x_max, newLeaf->mbr.y_min, newLeaf->mbr.y_max);

    newLeaf->center = findCenter(newLeaf);
    printf("found center: %f, %f\n", newLeaf->center.x, newLeaf->center.y);
    newLeaf->area = findArea(newLeaf->mbr);
    printf("found area: %d\n", newLeaf->area);
    return newLeaf;
}

NODE createNode(int index,  NODE *leavesList , int size)
{
    NODE newInternalNode;
    newInternalNode = malloc(sizeof(struct node));
    newInternalNode->count = 0;
    newInternalNode->type = 2;

    for(int i = 0; i< 4 && index<size; i++){
        newInternalNode->node_children[i] = leavesList[index];
        newInternalNode->rect[i] = (struct rect){.x_min = leavesList[index]->mbr.x_min, .y_min = leavesList[index]->mbr.y_min, .x_max = leavesList[index]->mbr.x_max, .y_max = leavesList[index]->mbr.y_max};
        index++;
        newInternalNode->count++;
    }

    newInternalNode->mbr = findMBR(newInternalNode);
    printf("found mbr: %d, %d, %d, %d...node", newInternalNode->mbr.x_min, newInternalNode->mbr.x_max, newInternalNode->mbr.y_min, newInternalNode->mbr.y_max);
    newInternalNode->center = findCenter(newInternalNode);
    printf("found center: %f, %f", newInternalNode->center.x, newInternalNode->center.y);
    newInternalNode->area = findArea(newInternalNode->mbr);
    printf("found area: %d\n", newInternalNode->area);

    return newInternalNode;
}

NODE* createLevel(NODE *leavesList, int size){

    int no_of_nodes = ceil(size/4.0);
    NODE *nodesList = malloc(no_of_nodes*sizeof(NODE));
    int index=0;
    for(int i =0; i<no_of_nodes; i++){
        nodesList[i] = createNode(index, leavesList, size);
        index += 4;
    }
    nodesList = sorting(nodesList, no_of_nodes);

    printf("=============\n");
    printf("%d", no_of_nodes);
    for(int i=0; i<no_of_nodes;i++){
        printf("size of level %d = %d\n", i, nodesList[i]->count);
    
    return nodesList ;
}
}

struct rtree *generateTree(NODE *leavesList, int count)
{
    struct rtree *tree = malloc(sizeof(struct rtree));
    // tree->root = malloc(sizeof(struct node));
    tree->count=count +1;
    tree->height=1;
    createTree(tree, leavesList, count);
    
    printTree(tree);

    preorder(tree->root,tree->root->count);
}

void createTree(struct rtree *tree, NODE *leavesList, int count)
{
    int x = ceil(count / (4.0));
    if (x == 1)
    {  
        tree->root = createNode(0, leavesList, count);
    }
    else
    {
        printf("creating nodes\n");
        NODE *nodesList = malloc(sizeof(NODE) * x);
        nodesList = createLevel(leavesList, count);
        tree->height++;
        tree->count += x;
        createTree(tree, nodesList, x);
    }
}

int findArea(struct rect mbr)
{
        return (mbr.x_max - mbr.x_min) * (mbr.y_max - mbr.y_min);
}

struct center findCenter(NODE given_node)
{
        struct center coord;
        coord.x = (given_node->mbr.x_min + given_node->mbr.x_max) / 2.0;
        coord.y = (given_node->mbr.y_min + given_node->mbr.y_max) / 2.0;
        return coord;
}


struct rect findMBR(NODE given_node)
{
    struct rect mbr = given_node->rect[0];
    for (int i = 1; i < given_node->count; i++)
    {
        if(given_node->type == 1){
            mbr.x_min = min(mbr.x_min, given_node->entries[i].x);
            mbr.y_min = min(mbr.y_min, given_node->entries[i].y);
            mbr.x_max = max(mbr.x_max, given_node->entries[i].x);
            mbr.y_max = max(mbr.y_max, given_node->entries[i].y);
        }else{
            mbr.x_min = min(mbr.x_min, given_node->node_children[i]->center.x);
            mbr.y_min = min(mbr.y_min, given_node->node_children[i]->center.y);
            mbr.x_max = max(mbr.x_max, given_node->node_children[i]->center.x);
            mbr.y_max = max(mbr.y_max, given_node->node_children[i]->center.y);
        }
    }
    return mbr;
}

void printTree(struct rtree* tree){
    printf("(%d, %d)\n", tree->root->area, tree->count);
    int n = tree->root->count;
    printf("height of tree: %d\n", tree->height);

    // printf("%d\n", tree->root->count);
    // printf("%d ", tree->root->node_children[0]->count);
    // printf("%d\n", tree->root->node_children[1]->count);
    // printf("%d ", tree->root->node_children[0]->node_children[0]->count);
    // printf("%d ", tree->root->node_children[0]->node_children[1]->count);
    // printf("%d ", tree->root->node_children[0]->node_children[2]->count);
    // printf("%d   ", tree->root->node_children[0]->node_children[3]->count);
    // printf("%d ", tree->root->node_children[1]->node_children[0]->count);
    // printf("%d \n", tree->root->node_children[1]->node_children[1]->count);
    // printf("%d   ", tree->root->node_children[1]->node_children[1]->entries[3].y);

    // printf();
}

// preorder traversal
void preorder(NODE root, int index)
{
    if (root->type == 1)
    {
            printf("2D objects in Leaf(external) nodes: ");
        for (int j = 0; j < root->count; j++)
        {
            printf(" (%d, %d)  ", root->entries[j].x, root->entries[j].y);
        }
            printf("\n");
        return;
        // order(root->entries, nodesList, index+1);
    }
    else if (root->type == 2){
        printf("Internal node mbr:- bottom left:(%d, %d), top right:(%d, %d)\n", root->mbr.x_min, root->mbr.x_max, root->mbr.y_min, root->mbr.y_max);
        for(int i = 0; i<root->count; i++){

            preorder(root->node_children[i],root->node_children[i]->count);
        }
    }
    return;
}

int main(int argc, char *argv[])
    {
        int data_size = 21;
        struct data data_entries[21];
        int P = ceil(data_size / (double)M);
        int S = ceil(sqrt(P));
        printf("%d, %d......p ns\n", P, S);

        // reading the data
        FILE *fp;
        fp = fopen("data.txt", "r");
        if (fp == NULL)
        {
            printf("Error opening file");
            exit(1);
        }
        int i = 0;
        while (!feof(fp))
        {
            int x, y;
            fscanf(fp, "%d %d", &data_entries[i].x, &data_entries[i].y);
            i++;
        }
        fclose(fp);

        // implementing str

        // applying heapsort to sort based on x and y
        Heap h = heap_create();
        h->points = malloc(sizeof(struct data));
        h->points = data_entries;

        h = build_max_heap(h, 0, 21); // building max heap
        h = heap_sort(h, 0, 21);      // sorting based on x

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
            int ele = min(S * M, remaining);

            Heap ySortingHeap = heap_create();
            ySortingHeap->points = malloc(sizeof(struct data) * ele);

            for (int i = k * S * M, l = 0; i < min((k + 1) * S * M, data_size) - k, l < ele; i++, l++)
            {
                ySortingHeap->points[l] = data_entries[i];
                printf("(%d, %d )", ySortingHeap->points[l].x, ySortingHeap->points[l].y);
            }
            printf("__\n");

            ySortingHeap = build_max_heap(ySortingHeap, 1, ele); // building max heap
            printf("********************888\n");
            ySortingHeap = heap_sort(ySortingHeap, 1, ele); // sorting based on y

            for (int j = 0; j < ele; j++)
            {
                printf("(%d, %d )", ySortingHeap->points[j].x, ySortingHeap->points[j].y);
            }
            printf("&&  after sort\n");
            for (int i = k * S * M, l = 0; i < min((k + 1) * S * M, data_size) - k, l < ele; i++, l++)
            {
                data_entries[i] = ySortingHeap->points[l];
                printf("(%d, %d )", data_entries[i].x, data_entries[i].y);
            }

            free(ySortingHeap->points);
            free(ySortingHeap);
            remaining = remaining - (S * M);
            printf("\n%d\n", remaining);
        }

        // creating list of leaves

        NODE *leaves_list = malloc(sizeof(NODE) * P);
        int r = data_size;
    for(i = 0 ; i<6; i++){
        leaves_list[i] = createLeaf(data_entries+i*4,r>4?4:r);
        r=r-4;
    }
    leaves_list = sorting(leaves_list, P);
    

    // creating tree
    generateTree(leaves_list, 6);

}

NODE* sorting(NODE* nodesList, int size)
{
    HeapNode h = heap_create_node();
    h->points = malloc(sizeof(NODE));
    h->points = nodesList;

    int P = ceil(size / (double)M);
    int S = ceil(sqrt(P));

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
        int ele = min(S * M, remaining);

        HeapNode ySortingHeap = heap_create_node();
        ySortingHeap->points = malloc(sizeof(struct node) * ele);

        for (int i = k * S * M, l = 0; i < min((k + 1) * S * M, size) - k, l < ele; i++, l++)
        {
            ySortingHeap->points[l] = nodesList[i];
            printf("(%f, %f )", ySortingHeap->points[l]->center.x, ySortingHeap->points[l]->center.y);
        }
        printf("__\n");

        ySortingHeap = build_max_heap_nodes(ySortingHeap, 1, ele); // building max heap
        printf("********************888\n");
        ySortingHeap = heap_sort_nodes(ySortingHeap, 1, ele); // sorting based on y

        for (int j = 0; j < ele; j++)
        {
            printf("(%f, %f )", ySortingHeap->points[j]->center.x, ySortingHeap->points[j]->center.y);
        }
        printf("&&  after sort\n");
        for (int i = k * S * M, l = 0; i < min((k + 1) * S * M, size) - k, l < ele; i++, l++)
        {
            nodesList[i] = ySortingHeap->points[l];
            // printf("(%d, %d )", nodesList[i].x,data_entries[i].y);
        }

        free(ySortingHeap->points);
        free(ySortingHeap);
        remaining = remaining - (S * M);
        printf("\n%d\n", remaining);
    }
    return nodesList;
}