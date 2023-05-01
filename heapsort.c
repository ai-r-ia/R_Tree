#include "rtree.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

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

int parent(Heap h, int node, int coord){
    int parent;
        parent = floor(node/2);
    if(parent >= 0){
        return parent;
    }else{
        return node;
    }
}

int left_child(Heap h, int node, int coord){
    int left;
        left = 2*(node) + 1;
    if(left < h->size){
        return left;
    }else{
        return node;
    }
}

int  right_child(Heap h, int node, int coord){
    int right;
        right = 2*(node) + 2;
    if(right < h->size){
        return right;
    }else{
        return node;
    }
}

void max_heapify(Heap h, int index, int coord)
{
    int left = left_child(h, index, coord);
    int right = right_child(h, index, coord);
    int largest = index;
    if(coord == 0){
        if (left < h->size &&  h->points[left].x > h->points[largest].x)
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

Heap build_max_heap(Heap h, int coord, int size){
    h->size = size;
    for(int i = floor(h->size / 2) ; i>=0; i--)
    {
        max_heapify(h, i, coord);
    }
    return h;
}

Heap heap_sort(Heap h, int coord, int size)
{
    h = build_max_heap(h, coord, size);
    for (int i = h->size - 1; i >=1; i--)
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
    int left = left_child(h, index, coord);
    int right = right_child(h, index, coord);
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
        struct node* temp = h->points[index];
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
        struct node* temp = h->points[0];
        h->points[0] = h->points[i];
        h->points[i] = temp;
        h->size = h->size - 1;
        max_heapify_nodes(h, 0, coord);
    }
    return h;
}
