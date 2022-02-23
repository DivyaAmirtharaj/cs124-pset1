#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include <math.h>

typedef struct vertex{
    float* location;
    int dimension;
} vertex;

typedef struct adjNode{
    int id;
    float weight;
    struct adjNode* next;
} adjNode;

adjNode* createAdjList(i){
    adjNode* root = malloc(sizeof(adjNode));
	root->next = NULL;
	root->weight = INT_MIN;
    root->id = i;
	return root;
}

adjNode* elIntoList(int id, float weight, adjNode* root){
    adjNode* temp = malloc(sizeof(adjNode));
    temp->id = id;
    temp->weight = weight;
    temp->next = root->next;
    root->next = temp;
    return temp;
}

float dist(vertex p1, vertex p2){ 
	float distance = 0.0;
	for(int i = 0; i < p1.dimension; i++){
		distance += (p1.location[i] - p2.location[i]) * (p1.location[i] - p2.location[i]); 
	}
	return sqrt(distance);
}

void print_node(adjNode* n){
	if(n == NULL)
		printf("This node was NULL\n");
	else
		printf("%d, %f\n", n->id, n->weight);
}

void print_lst(adjNode* root){
	adjNode* current = root->next;
	while(current != NULL){
		print_node(current);
		current = current->next;
	}
	printf("\n");
	return;
}


int main(int argc, char* argv[]){
	if(argc != 5){
		printf("Please add inputs: flag, numpoints, numtrials, dimension");
		return 1;
	}
	if(atoi(argv[4]) < 0){
		printf("Dimension is not valid");
		return 1;
	}
	// Set command line inputs
	int flag = atoi(argv[1]);
	int numpoints = atoi(argv[2]);
	int numtrials = atoi(argv[3]);
	int dimension = atoi(argv[4]);

    //array of length numpoints such that each element in the array is the linked list of edges
    adjNode* edgeArr [numpoints];
    //store all the points
    vertex points [numpoints];


    //initialize the adjacency lists
    for(int i = 0; i < numpoints; i++){
		edgeArr[i] = createAdjList(i);
	}

    //generate the random locations for the vertices if dimension != 0
    if (dimension >= 2 && dimension <= 4){
        //iterate through every vertex and assign it a "coordinate location"
        for (int i = 0; i < numpoints; i++){
            float *loc = malloc(sizeof(float) * dimension);
            for (int j = 0; j < dimension; j++){
                loc[j] = (float)rand() / (float)RAND_MAX;
            }
            points[i].dimension = dimension;
            points[i].location = loc;
            for (int a = 0; a < dimension; a++){
                printf("%f ", points[i].location[a]);
            } 
        }
    }


    //develop the adjacency list
    //iterate through each vertex
    for(int i = 0; i < numpoints; i++){
        //for each vertex, go through every other vertex and make an edge
        //note we only start with the vertex we're currently on and higher index vertices
        for (int j = i; j < numpoints; j++){
            float weight_check = 0;
            adjNode* node_check;
            //we don't want an edge from one node to the same node
            if (i == j){
                weight_check = 0;
            }
            //if dimension == 0, then we randomly assign the weight
            else if (dimension == 0){
                weight_check = (float) rand() / (float) RAND_MAX;
            }
            //otherwise, find the distances between the points
            else{
                weight_check = dist(points[i], points[j]);
            }
            //make two edges, one for outgoing and one ingoing since this is an undirected graph
            if(weight_check != 0){
                elIntoList(i, weight_check, edgeArr[j]);
                elIntoList(j, weight_check, edgeArr[i]);
            }
        }
        
    }
    for(int i = 0; i < numpoints; i++){
		printf("Edges from %d: ", i);
		print_lst(edgeArr[i]);
	}

}

