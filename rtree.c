#include<math.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "rtree.h"
#include "heapsort.c"

#define min(a,b) (((a) < (b)) ? (a) : (b))
#define max(a,b) (((a) > (b)) ? (a) : (b))
#define _ceil(n) ()

NODE createLeaf(struct data* data_entries, int count){

    NODE newLeaf;
    newLeaf = malloc(sizeof(struct node));
    newLeaf->count=0;
    newLeaf->type = 1;

    for(int i=0; i<count;i++){
        newLeaf->entries[i] = data_entries[i];
        newLeaf->count++;
    }

    newLeaf->mbr = findMBR(newLeaf);
    newLeaf->center = findCenter(newLeaf);
    newLeaf->area = findArea(newLeaf->mbr);

    return newLeaf;
}

NODE createNode(int index,  NODE *leavesList , int size)
{
    NODE newInternalNode;
    newInternalNode = malloc(sizeof(struct node));
    newInternalNode->count = 0;
    newInternalNode->type = 2;

    for(int i = 0; i< 4 && index<size; i++){
        newInternalNode->node_children[i] = leavesList[index++];
        newInternalNode->count++;
    }

    newInternalNode->mbr = findMBR(newInternalNode);
    newInternalNode->center = findCenter(newInternalNode);
    newInternalNode->area = findArea(newInternalNode->mbr);

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
    tree->root = malloc(sizeof(struct node));
    tree->count=count +1;
    tree->height=1;
    createTree(tree, leavesList, count);

    printTree(tree);

    // preorder(tree->root,tree->count);
}

void createTree(struct rtree *tree, NODE *leavesList, int count)
{
    int x = ceil(count / (4.0));
    if (x == 1)
    {
        for (int i = 0; i < count; i++)
        {
            tree->root->node_children[i] = leavesList[i];
        }
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

struct data findCenter(NODE given_node)
{
        struct data coord;
        coord.x = (given_node->mbr.x_min + given_node->mbr.x_max) / 2.0;
        coord.y = (given_node->mbr.y_min + given_node->mbr.y_max) / 2.0;
        return coord;
}

NODE *algo_str(NODE *leaveslist, int size)
{
        int P = ceil( size / (double) M);
        int S = ceil(sqrt(P));
        printf("%d, %d", P, S);
        struct data coords[6];
        for (int i = 0; i < size; i++)
        {
        coords[i] = findCenter(leaveslist[i]);
        }

      
}

struct rect findMBR(NODE given_node)
{
    struct rect mbr = given_node->rect[0];
    for (int i = 1; i < given_node->count; i++)
    {
        mbr.x_min = min(mbr.x_min, given_node->rect[i].x_min);
        mbr.y_min = min(mbr.y_min, given_node->rect[i].y_min);
        mbr.x_max = max(mbr.x_max, given_node->rect[i].x_max);
        mbr.y_max = max(mbr.y_max, given_node->rect[i].y_max);
    }
    return mbr;
}

void printTree(struct rtree* tree){
    printf("(%d, %d)\n", tree->root->area, tree->count);
    int n = tree->root->count;
    printf("height of tree: %d\n", tree->height);

    printf("%d\n", tree->root->count);
    printf("%d ", tree->root->node_children[0]->count);
    printf("%d\n", tree->root->node_children[1]->count);
    printf("%d ", tree->root->node_children[0]->node_children[0]->count);
    printf("%d ", tree->root->node_children[0]->node_children[1]->count);
    printf("%d ", tree->root->node_children[0]->node_children[2]->count);
    printf("%d   ", tree->root->node_children[0]->node_children[3]->count);
    printf("%d ", tree->root->node_children[1]->node_children[0]->count);
    printf("%d \n", tree->root->node_children[1]->node_children[1]->count);
    printf("%d   ", tree->root->node_children[1]->node_children[1]->entries[0].y);

    // printf();
}


void order(NODE root, NODE* nodesList, int index)
{
    if (root == NULL)
    {
        return;
    }
    nodesList[index] = malloc(sizeof(struct node));
    nodesList[index] = root;
    for (int i = 0; i < root->count; i++)
    {
        if(root->type == 2){
            order(root, nodesList, index+1);
        }
        else{
            for(int j  = 0; j<  root->count; j++){
                printf("%d, %d | ", root->entries[j].x, root->entries[j].y);
            }
            order(root, nodesList, index+1);
        }
    }
    return;
}

// preorder traversal
void preorder(NODE root, int n)
{
    NODE nodesList = malloc(sizeof(struct node) * n);
    order(root, nodesList,0);
    for (int i = 0; i< n; i++){
        // printf("%d....\n", i);
        printf("%d\n", nodesList[i].count);
    }   
}   

int main(int argc, char *argv[]){
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
    int i=0;
    while(!feof(fp)){
        int x,y ;
        fscanf(fp,"%d %d",&data_entries[i].x,&data_entries[i].y);
        i++;
    }
    fclose(fp);

    // implementing str
    
    // applying heapsort to sort based on x and y
    Heap h = heap_create();
    h->points = malloc(sizeof(struct data));
    h->points = data_entries;
    
    h = build_max_heap(h, 0, 21);            //building max heap
    h = heap_sort(h, 0, 21);                 //sorting based on x

    for(int i = 0; i<21; i++){
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
        ySortingHeap->points = malloc(sizeof(struct data)* ele);

        for (int i = k * S * M, l = 0; i < min((k + 1) * S * M, data_size)-k, l<ele; i++, l++)
        {
            ySortingHeap->points[l] = data_entries[i];
            printf("(%d, %d )", ySortingHeap->points[l].x, ySortingHeap->points[l].y);
        }
        printf("__\n");

        ySortingHeap = build_max_heap(ySortingHeap, 1, ele); // building max heap
        printf("********************888\n");
        ySortingHeap = heap_sort(ySortingHeap, 1, ele);      // sorting based on y



        for (int j = 0; j < ele; j++)
        {
            printf("(%d, %d )", ySortingHeap->points[j].x, ySortingHeap->points[j].y);
        }
        printf("&&  after sort\n");
        for (int i = k * S * M, l = 0; i < min((k + 1) * S * M, data_size) - k, l < ele; i++, l++)
        {
            data_entries[i] = ySortingHeap->points[l];
            printf("(%d, %d )", data_entries[i].x,data_entries[i].y);
        }

        free(ySortingHeap->points);
        free(ySortingHeap);
        remaining = remaining -( S * M);
        printf("\n%d\n", remaining);
    }


   

    // creating list of leaves

    NODE leaves_list[6];
        int r = 21;
    for(i = 0 ; i<6; i++){
        leaves_list[i] = createLeaf(data_entries+i*4,r>4?4:r);
        r=r-4;
    }
    

    // creating tree
    generateTree(leaves_list, 6);

}