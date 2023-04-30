#include "rtree.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>



NODE createLeaf(struct data* data_entries, int count){
    NODE new_leaf;
    new_leaf = malloc(sizeof(struct node));
    // printf("----%d-----\n",count);
    new_leaf->count=0;
    new_leaf->kind = 1;
    for(int i=0; i<count;i++){
        new_leaf->entries[i] = data_entries[i];
        new_leaf->count++;
    }
    return new_leaf;
}

NODE* createNodes(NODE *leavesList, int size){
    int no_of_nodes = ceil(size/4.0);
    // printf("%d  ", no_of_nodes);
    NODE *nodesList = malloc(no_of_nodes*sizeof(NODE)) ;
    int j=0;
    for(int i =0; i<no_of_nodes; i++){
        nodesList[i]->count=0;
        nodesList[i]->kind=2;
        if(j==size) {break;}
        for(int k=0;k<4;k++){
        if(j==size) {break;}
        nodesList[i]->node_children[k] = leavesList[j++] ; 
        nodesList[i]->count++ ;
        

        // printf("adding onto level %d - node %d\n", i,j);
        }
        // printf("size of level %d = %d\n", i, nodesList[i]->count);
    }
    printf("=============\n");
    for(int i=0; i<no_of_nodes;i++){
        printf("size of level %d = %d\n", i, nodesList[i]->count);
    // }
    // printf("size of level 1= %d\n" , nodesList[1]->node_children[1]->entries[0].y);

    
    return nodesList ;
}
}

void createTree(struct rtree *tree, NODE *leavesList, int count){
    int x = ceil(count/(4.0)); 
    if(x==1){
        for(int i=0; i<count; i++){
            // printf("----%d---\n",i);
            tree->root->node_children[i] = leavesList[i];
        }
    }
    else{
        printf("creating nodes\n");
        NODE* nodesList = malloc(sizeof(NODE)*x);
        nodesList = createNodes(leavesList,count);
        // printf("count at level 1= %d\n", nodesList[1]->count);
        createTree(tree,nodesList,x);
        }
}

int main(int argc, char *argv[]){

struct data data_entries[21];

// reading the data
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

// printing the data entries
    // for(i=0;i<21;i++){
    //     printf("%d %d\n",data_entries[i].x,data_entries[i].y);
    // }

// implementing str

// sorting based on values of x
struct data swap;
for(int j=0; j<20;j++)
for(i =0 ; i<20-j; i++){
    if(data_entries[i].x > data_entries[i+1].x){
        swap = data_entries[i];
        data_entries[i]=data_entries[i+1];
        data_entries[i+1] = swap;
    }
}

// for(i=0;i<21;i++){
//         printf("%d %d\n",data_entries[i].x,data_entries[i].y);
//     }

int max_cap = 4;

// sorting based on values of y

for(int j=0; j<11;j++)
for(i =0 ; i<11-j; i++){
    if(data_entries[i].y > data_entries[i+1].y){
        swap = data_entries[i];
        data_entries[i]=data_entries[i+1];
        data_entries[i+1] = swap;
    }
}

// for(i=0;i<21;i++){
//         printf("%d %d\n",data_entries[i].x,data_entries[i].y);
//     }


for(int j=0; j<9;j++)
for(i =12 ; i<20-j; i++){
    if(data_entries[i].y > data_entries[i+1].y){
        swap = data_entries[i];
        data_entries[i]=data_entries[i+1];
        data_entries[i+1] = swap;
    }
}

// creating list of leaves

NODE leaves_list[6];
int r = 21;
for(i = 0 ; i<6; i++){
leaves_list[i] = createLeaf(data_entries+i*4,r>4?4:r);
r=r-4;
}

// for(i=0 ; i<5;i++){
//     for(int j=0;j<4;j++)
//     printf("%d %d\n", leaves_list[i]->entries[j].x,leaves_list[i]->entries[j].y);
//     printf("-----\n");
// }
//  for(int j=0;j<1;j++)
//     printf("%d %d\n", leaves_list[5]->entries[j].x,leaves_list[5]->entries[j].y);
//     printf("-----\n");


int *nodeCentreList;
int number_of_nodes; //needs to be initialized 
nodeCentreList = (struct nodeCentre*) malloc(sizeof(struct nodeCentre)*number_of_nodes); //take number of nodes waala number here


// creating tree

struct rtree* tree = malloc(sizeof(struct rtree));
tree->root = malloc(sizeof(struct node));
createTree(tree,leaves_list,6);

// printf("%d\n",tree->root->count);
printf("%d ",tree->root->node_children[0]->count);
printf("%d\n",tree->root->node_children[1]->count);
printf("%d ",tree->root->node_children[0]->node_children[0]->count);
printf("%d ",tree->root->node_children[0]->node_children[1]->count);
printf("%d ",tree->root->node_children[0]->node_children[2]->count);
printf("%d   ",tree->root->node_children[0]->node_children[3]->count);
printf("%d ",tree->root->node_children[1]->node_children[0]->count);
printf("%d \n",tree->root->node_children[1]->node_children[1]->count);
printf("%d   ",tree->root->node_children[1]->node_children[1]->entries[0].y);
}