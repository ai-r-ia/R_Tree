#include"rtree.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include <stdbool.h>

int maxi(int a, int b){
    return a>b?a:b;
}

int mini(int a, int b){
    return a>b?b:a;
}

int findArea(struct rect mbr){
    return (mbr.x_max-mbr.x_min)*(mbr.y_max-mbr.y_min);
}


struct data finddata(NODE given_node){
    struct data coord;
    coord.x = (given_node->mbr.x_min+given_node->mbr.x_max)/2.0;
    coord.y = (given_node->mbr.y_min+given_node->mbr.y_max)/2.0;
    // printf("%f %f\n", coord.x , coord.y);
    return coord ;
}

// NODE* algo_str(NODE* leaveslist, int size){
//     int P = ceil(size/M);
//     int S = ceil(sqrt(P));
//     struct data coords[6];
//     for(int i=0 ;i<size;i++){
//         coords[i] = finddata(leaveslist[i]);
//     }
    
//     // for(int i=0 ;i<size;i++){
//     //    printf("%f %f ",coords[i].x,coords[i].y);
//     // }
    

//     // sorting based on values of x
//     NODE swap =malloc(sizeof(struct node));
//     for(int j=0; j<size-1;j++)
//     for(int i =0 ; i<size-1-j; i++){
//         if(coords[i].x > coords[i+1].x){
//             swap = leaveslist[i];
//             leaveslist[i]=leaveslist[i+1];
//             leaveslist[i+1] = swap;
//         }
//     }
//     return leaveslist;

    // // sorting based on values of y
    // int remaining = data_size;
    // i=0;
    // for(int k =0 ; k<S ; k++){ 
    //     if(remaining <=0) {break;}  
    //     for(int j=0; j<min(S*M,remaining);j++){
    //         for(i=k*S*M ; i<min((k+1)*S*M,data_size)-1-j; i++){
    //             if(data_entries[i].y > data_entries[i+1].y){
    //                 swap = data_entries[i];
    //                 data_entries[i]=data_entries[i+1];
    //                 data_entries[i+1] = swap;
    //             }
    //         }
    //     }
    //      remaining = remaining - S*M;
    // }
// }

struct rect findMBR(NODE given_node){
    struct rect mbr = given_node->rect[0];
    for(int i=1; i<given_node->count;i++){
        mbr.x_min = mini(mbr.x_min, given_node->rect[i].x_min);
        mbr.y_min = mini(mbr.y_min, given_node->rect[i].y_min);
        mbr.x_max = maxi(mbr.x_max, given_node->rect[i].x_max);
        mbr.y_max = maxi(mbr.y_max, given_node->rect[i].y_max);
    }
    return mbr;
}


// creating leaves
NODE createLeaf(struct data* data_entries, int count){
    NODE new_leaf;
    new_leaf = malloc(sizeof(struct node));
    new_leaf->count=0;
    new_leaf->type = 1;
    for(int i=0; i<count;i++){
        new_leaf->entries[i] = data_entries[i];
        new_leaf->rect[i] = (struct rect){.x_min=data_entries[i].x,.y_min=data_entries[i].y, .x_max=data_entries[i].x,.y_max=data_entries[i].y}  ;
        new_leaf->count++;
    }
    new_leaf->mbr = findMBR(new_leaf);
    return new_leaf;
}

// creating nodes
NODE* createNodes(NODE *leavesList, int size){
    int no_of_nodes = ceil(size/4.0);
    NODE *nodesList = malloc(no_of_nodes*sizeof(NODE)) ;
    int j=0;
    for(int i =0; i<no_of_nodes; i++){
        nodesList[i]->count=0;
        nodesList[i]->type=2;
        for(int k=0;k<4;k++){
        if(j==size) {break;}
        nodesList[i]->node_children[k] = leavesList[j] ; 
        nodesList[i]->count++ ;  
        nodesList[i]->rect[k] = leavesList[j++]->mbr; 
        }
        nodesList[i]->mbr = findMBR(nodesList[i]);
    }
    return nodesList ;

}

// creating tree
void createTree(struct rtree *tree, NODE *leavesList, int count){
    int x = ceil(count/(4.0)); 
    tree->height++;
    if(x==1){
        for(int i=0; i<count; i++){
            tree->root->node_children[i] = leavesList[i];
            tree->root->rect[i] = leavesList[i]->mbr;
        }
        tree->root->count = count;
        tree->root->mbr=findMBR(tree->root);
    }
    else{
        printf("creating nodes\n");
        NODE* nodesList = malloc(sizeof(NODE)*x);
        nodesList = createNodes(leavesList,count);

        createTree(tree,nodesList,x);
        }
}

//karna hai
bool intersects(){ 
    return false;
}

bool searchnode(NODE given_node, int xmin, int xmax, int ymin, int ymax){
    if(given_node->type==1){

        for (int i = 0 ; i < given_node->count ; i++){

            if ((given_node->entries[i].x <= xmax) && (given_node->entries[i].x >= xmin)&&(given_node->entries[i].y <= ymax)&&(given_node->entries[i].y >=ymin)){
                
                printf("x coordinate : %d y:  %d\n", given_node->entries[i].x, given_node->entries[i].y);
            }
        
        }


        return true;

    }

    else{
        for (int i = 0 ; i < given_node->count ; i++){
            if (intersects()) //karna hai
                searchNode(given_node->node_children[i], xmin, xmax, ymin, ymax);
        }


    }

    
}

void searchTree(struct rtree* tree, int xmin, int xmax, int ymin, int ymax){
    if(tree->root){
        searchNode(tree->root, xmin, xmax, ymin, ymax);
    }
}

// main function
// int main(int argc, char *argv[]){

//     int data_size = 21;
//     struct data data_entries[21];

//     // reading the data
//     FILE *fp;
//         fp = fopen("data.txt", "r");
//         if(fp == NULL)
//         {
//             printf("Error opening file");
//             exit(1);
//         }
//         int i=0;
//         while(!feof(fp)){
//             int x,y ;
//             fscanf(fp,"%d %d",&data_entries[i].x,&data_entries[i].y);
//             i++;
//     }
//         fclose(fp);

//     int P = ceil(data_size/M);
//     int p=0;
//     int S = ceil(sqrt(p*1.0));
    
//     // implementing str

//     // sorting based on values of x
//     struct data swap;
//     for(int j=0; j<data_size-1;j++){
//     for(i =0 ; i<data_size-1-j; i++){
//         if(data_entries[i].x > data_entries[i+1].x){
//             swap = data_entries[i];
//             data_entries[i]=data_entries[i+1];
//             data_entries[i+1] = swap;
//         }
//     }
//     }

//     // sorting based on values of y
//     int remaining = data_size;
//     i=0;
//     for(int k =0 ; k<S ; k++){ 
//         if(remaining <=0) {break;}  
//         for(int j=0; j<min(S*M,remaining);j++){
//             for(i=k*S*M ; i<min((k+1)*S*M,data_size)-1-j; i++){
//                 if(data_entries[i].y > data_entries[i+1].y){
//                     swap = data_entries[i];
//                     data_entries[i]=data_entries[i+1];
//                     data_entries[i+1] = swap;
//                 }
//             }
//         }
//          remaining = remaining - S*M;
//     }

// // lite
//     // for(int j=0; j<11;j++)
//     // for(i =0 ; i<11-j; i++){
//     //     if(data_entries[i].y > data_entries[i+1].y){
//     //         swap = data_entries[i];
//     //         data_entries[i]=data_entries[i+1];
//     //         data_entries[i+1] = swap;
//     //     }
//     // }

//     // for(int j=0; j<9;j++)
//     // for(i =12 ; i<20-j; i++){
//     //     if(data_entries[i].y > data_entries[i+1].y){
//     //         swap = data_entries[i];
//     //         data_entries[i]=data_entries[i+1];
//     //         data_entries[i+1] = swap;
//     //     }
//     // }

//     // creating list of leaves

//     NODE leaves_list[6];
//     int r = 21;
//     for(i = 0 ; i<6; i++){
//     leaves_list[i] = createLeaf(data_entries+i*4,r>4?4:r);
//     r=r-4;
//     }
//     r=21;

//     // NODE* leaves_list2 = malloc(sizeof(NODE)*6);
//     // leaves_list2 = algo_str(leaves_list, 6);
//     // for(i =0; i<6;i++){
//     //     for(int j =0; j<leaves_list2[i]->count; j++){
//     //         printf("%d %d\n", leaves_list2[i]->entries[j].x,leaves_list2[i]->entries[j].y);
//     //     }
//     //     printf("------------\n");
//     // }+
//     // creating tree

//     // struct rtree* tree = malloc(sizeof(struct rtree));
//     // tree->root = malloc(sizeof(struct node));
//     // tree->height = r>0?1:0;
//     // createTree(tree,leaves_list,6);
//     // printf("done\n");

//     // printf("%d %d\n", tree->root->node_children[1]->node_children[1]->entries[0].x,tree->root->node_children[1]->node_children[1]->entries[0].y);
// }