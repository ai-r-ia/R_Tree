#include "rtree.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

Heap heap_create()
{
    Heap h = malloc(sizeof(struct heap));
    h->points = malloc(sizeof(struct data));
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

    else if(coord == 1){
             if (left < h->size && h->points[left].x != h->points[largest].x)
            {
                if( h->points[left].x > h->points[largest].x || h->points[left].y > h->points[largest].y){
                    largest = left;
                }
                
            }
            if (right < h->size  &&  h->points[right].x >= h->points[largest].x)
            {
                 if( h->points[right].x > h->points[largest].x || h->points[right].y > h->points[largest].y){
                    largest = right;
                }
            
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

Heap build_max_heap(Heap h, int coord){
    h->size = 21;
    for(int i = floor(h->size / 2) ; i>=0; i--)
    {
        max_heapify(h, i, coord);
    }
    return h;
}

Heap heap_sort(Heap h, int coord)
{
    h = build_max_heap(h, coord);
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

int main(void){
    struct data data_entries[21];
    FILE *fp;
    fp = fopen("data.txt", "r");
    if(fp == NULL)
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

    Heap h = malloc(sizeof(struct heap));
    h->points = data_entries;
    
    h = build_max_heap(h, 0);
    //  printf("%d ", sizeof(h->points)/sizeof(h->points[0]));
    int n= h->size;
    h = heap_sort(h, 0);
    h = heap_sort(h, 1);
    // heap_sort(h, 0);
    for(int i = 0; i<n; i++){
         printf("(%d, %d) ", h->points[i].x, h->points[i].y);
    }
}

// (2,20)(2,19)-----> issue

//output
// (1, 9) (1, 20) (2, 4) (2, 5) (2, 10) (2, 20) (2, 19) (3, 4) (3, 5) (3, 20) (4, 5)
//  (7, 15) (8, 5) (8, 14) (8, 15) (9, 14) (9, 15) (9, 16) (9, 17) (11, 18) (12, 17)