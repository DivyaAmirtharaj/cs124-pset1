#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include <math.h>


/************** Structs for Generating Graph ****************/
typedef struct vertex{
    float* location;
    int dimension;
} vertex;

// Used in adjacency list
typedef struct nodeEdge{
    int vertex;
    float weight;
    struct nodeEdge* next;
} nodeEdge;
/*********************************************************/

nodeEdge* createAdjList(i){
    nodeEdge* root = malloc(sizeof(nodeEdge));
	root->next = NULL;
	root->weight = INT_MIN;
    root->vertex = i;
	return root;
}

nodeEdge* elIntoList(int id, float weight, nodeEdge* root){
    nodeEdge* temp = malloc(sizeof(nodeEdge));
    temp->vertex = id;
    temp->weight = weight;
    temp->next = root->next;
    root->next = temp;
    return temp;
}
/***************************/

/************* Helper Functions for Graph Generation **************/

float dist(vertex p1, vertex p2){ 
	float distance = 0.0;
	for(int i = 0; i < p1.dimension; i++){
		distance += (p1.location[i] - p2.location[i]) * (p1.location[i] - p2.location[i]); 
	}
    distance = sqrt(distance);
    printf("%f \n", distance);
	return sqrt(distance);
}

void print_node(nodeEdge* n){
	if(n == NULL)
		printf("This node was NULL\n");
	else
		printf("%d, %f\n", n->vertex, n->weight);
}

void print_lst(nodeEdge* root){
	nodeEdge* current = root->next;
	while(current != NULL){
		print_node(current);
		current = current->next;
	}
	// printf("\n");
    // fflush(stdout);
	return;
}

/***************************/


// Used for a heap node vs the node in graph generation
typedef struct heapNode{
    int vertex;
    float weight;
} heapNode;

// Define what a bi heap is
typedef struct heap{
    int size;
    int capacity;
    heapNode** heap_array;
} heap;

/**************** Heap *****************/
// Actually build the min heap
heap* buildMinHeap() {
	heap* minHeap = (heap*)malloc(sizeof(heap));
	minHeap->size = 0;
	minHeap->capacity = 0;
	minHeap->heap_array = (heapNode**)malloc(minHeap->capacity * sizeof(heapNode*));
	return minHeap;
}

// Add a new node to the heap
void addNode(int vertex, float weight, heap* init_heap){
    if(init_heap->capacity != 0 && init_heap->capacity == init_heap->size){
		init_heap->heap_array = realloc(init_heap->heap_array, (init_heap->capacity * 2) * sizeof(heapNode));
		init_heap->capacity *= 2;
		init_heap->size += 1;
	}
	else if(init_heap->size == 0){
		init_heap->heap_array = malloc(sizeof(heapNode));
		init_heap->size += 1;
		init_heap->capacity += 1;
	}
	else{
		init_heap->size += 1;
	}
	heapNode* addNode = (heapNode*)malloc(sizeof(heapNode));
	addNode->vertex = vertex;
	addNode->weight = weight;

    int iterate_size = (init_heap->size - 1);
	while(iterate_size > 0 && addNode->weight < init_heap->heap_array[(iterate_size - 1)/2]->weight){
		init_heap->heap_array[iterate_size] = init_heap->heap_array[(iterate_size - 1)/2]; 
		iterate_size = (iterate_size - 1)/2;
	}
	init_heap->heap_array[iterate_size] = addNode;
}

// Min heap functions
// 1. heapify %
// 2. popMin %
// 3. fix heap, either decrease a key, add a key at the end, or delete a key

// Heapify
void fixHeap(heap* minHeap, int x){
	int left = 2*x+1;
    int right = 2*x+2;
    int smallest = x;
    int parent = (x-1)/2;

	if (left < minHeap->size && minHeap->heap_array[left]->weight < minHeap->heap_array[smallest]->weight)
		smallest = left;

	if (right < minHeap->size && minHeap->heap_array[right]->weight < minHeap->heap_array[smallest]->weight)
		smallest = right;

    if(smallest != x){
		heapNode* temp = minHeap->heap_array[x];
		minHeap->heap_array[x] = minHeap->heap_array[smallest];
		minHeap->heap_array[smallest] = temp;
		fixHeap(minHeap, smallest);
	}
}

/*void decrease(heap* minHeap, int vertex, int weight)
{
	int i = minHeap->position[vertex];
	minHeap->heap_array[i]->weight = weight;

	while (i && minHeap->heap_array[i]->weight < minHeap->heap_array[(i - 1) / 2]->weight) {
		// Swap this node with its parent
		minHeap->position[minHeap->heap_array[i]->vertex] = (i - 1) / 2;
		minHeap->position[minHeap->heap_array[(i - 1) / 2]->vertex] = i;
        heapNode* n = *&minHeap->heap_array[i];
        *&minHeap->heap_array[(i - 1) / 2] = n;
		i = (i - 1) / 2;
	}
}*/

// Pop minimium node
heapNode* popMin(heap* init_heap){

    if(init_heap->size > 0){
        heapNode *min = init_heap->heap_array[0];
        init_heap->size = init_heap->size - 1;
        init_heap->heap_array[0] = init_heap->heap_array[init_heap->size];
        fixHeap(init_heap, 0);
        if(init_heap->size <= init_heap->capacity / 2){
            init_heap->heap_array = realloc(init_heap->heap_array, (init_heap->capacity / 2) * sizeof(heapNode));
			init_heap->capacity /= 2;
        }
        return min;
    }
    else {
        free(init_heap);
        return NULL;
    }
}

nodeEdge* findCurrNode(int vertex, nodeEdge* init_edge){
	nodeEdge* curr_edge = init_edge->next;
	while(curr_edge != NULL && curr_edge->vertex != vertex){
		curr_edge = curr_edge->next;
	}
	if(curr_edge == NULL){
		return NULL;
	}
	return curr_edge;
}
/************* End of Heap *************/


/**************** Prim's Algorithm *****************/

float prim(int numpoints, nodeEdge* edges[numpoints]){
    // printf("check called\n");
    //initialize minHeap to store all vertices
	heap* init_heap = buildMinHeap();
	addNode(0, 0.0, init_heap);
    // printf("check add node \n");
    float weights[numpoints];
	int prev_node[numpoints];
	uint8_t diff[numpoints];

	// Initialize array  
	for(int i = 0; i < numpoints; i++){
		weights[i] = INT_MAX;
		prev_node[i] = INT_MIN;
		diff[i] = 1;
	}
    // printf("initialize array \n");
	weights[0] = 0;
    prev_node[0] = 0;
    // initialize adjacency list

    int size;
    size = 1;
	while(init_heap->size > 0){
        // printf("while start \n");
        // printf("%i\n", temp);
		heapNode* min = popMin(init_heap);
        // printf("plsss\n");
		init_heap->size = init_heap->size - 1;
		
        // printf("intermediate \n");
		int v = min->vertex;
		diff[v] = 0;
		nodeEdge* node_curr = edges[v]->next;
        // printf("problem 1\n");
		while(node_curr){
			int x = node_curr->vertex;
			if(diff[x] && (node_curr->weight < weights[x])){
				weights[x] = node_curr->weight;
				prev_node[x] = v;
				addNode(x, weights[x], init_heap);
				size = size+1;
			}
			node_curr = node_curr->next;
            // printf("problem 2\n");
		}
		free(min);
        // printf("problem 3\n");
	}
    // printf("after while \n");
    int not_in_mst = 0;
	float tot_weight = 0.0;
	//float largest_edge_used = 0.0;
	int largest_index = -3;
    // printf("sup dawg\n");
	for(int i = 1; i < numpoints; i++){
		if (prev_node[i] < 0){
			free(init_heap);
            //increase the count of edges not in the mst
			not_in_mst++;
			return -1;
		}
		nodeEdge* temp_edge = findCurrNode(prev_node[i], edges[i]);
        float temp_weight = temp_edge->weight;
		tot_weight += temp_weight;
	}
	free(init_heap);
	return tot_weight;
}

/************* End of Prim's Algorithm *************/

int main(int argc, char *argv[])
{
    if (argc != 5)
    {
        printf("Please add inputs: flag, numpoints, numtrials, dimension\n");
        return 1;
    }
    if (atoi(argv[4]) < 0)
    {
        printf("Dimension should be a positive integer\n");
        return 1;
    }
    // Set command line inputs
    int flag = atoi(argv[1]);
    int numpoints = atoi(argv[2]);
    int numtrials = atoi(argv[3]);
    int dimension = atoi(argv[4]);
    float sumTotal = 0.0;



    // start the CLOCK
    clock_t start = clock();



    //loop through the algorithm for the number of times we run the trial
    for (int count = 0; count < numtrials; count++)
    {
        srand(time(NULL));
        // array of length numpoints such that each element in the array is the linked list of edges
        nodeEdge *edgeArr[numpoints];
        // store all the points
        vertex points[numpoints];

        // initialize the adjacency lists
        for (int i = 0; i < numpoints; i++)
        {
            edgeArr[i] = createAdjList(i);
        }
        //assign random locations for vertices
        if (dimension >= 2 && dimension <= 4)
        {
            for (int trial = 0; trial < numtrials; trial++)
            {
                // generate the random locations for the vertices if dimension != 0

                // iterate through every vertex and assign it a "coordinate location"
                for (int i = 0; i < numpoints; i++)
                {
                    float *loc = malloc(sizeof(float) * dimension);
                    for (int j = 0; j < dimension; j++)
                    {
                        loc[j] = (float)rand() / (float)RAND_MAX;
                    }
                    points[i].dimension = dimension;
                    points[i].location = loc;
                    // for (int a = 0; a < dimension; a++){
                    //     printf("%f ", points[i].location[a]);
                    // }
                }
            }
        }
        // develop the adjacency list
        // iterate through each vertex
        for (int i = 0; i < numpoints; i++)
        {
            // for each vertex, go through every other vertex and make an edge
            // note we only start with the vertex we're currently on and higher index vertices
            for (int j = i; j < numpoints; j++)
            {
                float weight_check = 0;
                // we don't want an edge from one node to the same node
                if (i == j)
                {
                    weight_check = 10.0;
                }
                // if dimension == 0, then we randomly assign the weight
                else if (dimension == 0)
                {
                    weight_check = (float) rand() / (float)RAND_MAX;
                }
                // otherwise, find the distances between the points
                else
                {
                    weight_check = dist(points[i], points[j]);
                }
                // make two edges, one for outgoing and one ingoing since this is an undirected graph
                //  printf('%f', weight_check);
                if (weight_check < 1.0)
                {
                    elIntoList(i, weight_check, edgeArr[j]);
                    elIntoList(j, weight_check, edgeArr[i]);
                }
            }
        }
    

    // for (int i = 0; i < numpoints; i++)
    // {
    //     printf("Edges from %d: \n", i);
    //     print_lst(edgeArr[i]);
    // }
    // printf("Hello World\n");

    // typedef struct nodeEdge{
    //     int vertex;
    //     float weight;
    //     struct nodeEdge* next;
    // } nodeEdge;

    // nodeEdge* test1 [3];
    // for (int i = 0; i < 3; i++){
    //     test1 [i] = createAdjList(i);
    // }

    float total = prim(numpoints, edgeArr);
    sumTotal += total;

    // printf("Weight Size for trial %d: %f \n", (count + 1), total);
    }
    clock_t finish = clock();
    float averageTime = (float) (finish - start) / CLOCKS_PER_SEC;
    averageTime /= (float) numtrials;
    float averageTotal = sumTotal / (float) numtrials;
    float averageEdge = averageTotal / numpoints;
    printf("Average Total Size: %f \n", averageTotal);    
    printf("Average Edge Size in MST: %f \n", averageEdge);
    printf("Average Time: %f\n", averageTime);    
}
