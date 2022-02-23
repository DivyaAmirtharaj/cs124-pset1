#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

// Used in adjacency list
typedef struct nodeEdge{
    int node;
    float weight;
    struct nodeEdge* next;
} nodeEdge;

// Used in point generation
typedef struct vertex{
    int dim;
    float* location;
} vertex;

// Used for a heap node vs the node in graph generation
typedef struct MinHeapNode{
    
}

/**************** Prim's Algorithm *****************/

nodeEdge* 
 
/************* End of Prim's Algorithm *************/


int main(int argc, char* argv[]){
	if(argc != 5){
		printf("Please add inputs: flag, numpoints, numtrials, dimension");
		return 1;
	}
	if(atoi(argv[4]) <= 0){
		printf("Dimension should be a positive integer");
		return 1;
	}
	// Set command line inputs
	int flag = atoi(argv[1]);
	int numpoints = atoi(argv[2]);
	int numtrials = atoi(argv[3]);
	int dimension = atoi(argv[4]);

    /************************* Set Graph *************************/
    /* 
    Initialize set of points by looping through # of dimensions
    and looping through number of points to set the value for 
    each point in the graph
    */
    vertex vertices[numpoints];
    for(int i = 0; i < dimension; i++){
        float* vertex = malloc(sizeof(float)*dimension);
        for(int j = 0; j < numpoints; j++){
            vertex[j] = (float)rand() / INT_MAX;
            printf("This is i,j %d, %d\n", i, j);
            printf("This is coordinates %f\n", vertex[j]);
        }
    }

    /* 
    Initialize set of edges by creating adjacency list and setting
    the weight of each edge to the distance between the coordinates
    (position is given in coordinates)
    */
   /************************ End Set Graph ************************/
}


int primMST()