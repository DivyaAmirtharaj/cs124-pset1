#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

// Used in adjacency list
typedef struct nodeEdge{
    int vertex;
    float weight;
    struct nodeEdge* next;
} nodeEdge;

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

    int iterate_size = (init_heap->size - 1)/2;
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
    heapNode* min = init_heap->heap_array[0];
    init_heap->size = init_heap->size-1;
    init_heap->heap_array[0] = init_heap->heap_array[init_heap->size];
    fixHeap(init_heap, 0);
    return min;
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
/*
float prim(int numpoints, nodeEdge* edges[numpoints]){
	heap* init_heap = buildMinHeap();
	addNode(0, 0.0, init_heap);

    // dist: array[n] of weights
    float weights[numpoints];

    // prev: array[n] of vertices
	int prev_node[numpoints];
	uint8_t diff[numpoints];

	// Initialize array  
	for(int i = 0; i < numpoints; i++){
		weights[i] = INT_MAX;
		prev_node[i] = INT_MIN;
		diff[i] = 1;
	}
	weights[0] = 0;
    prev_node[0] = 0;
    // initialize adjacency list

    int size, count;
    size = 1;
    count = 0;
	while(init_heap->size > 0){
        \\ v:=deletemin(h)
		heapNode* min = popMin(init_heap);
		init_heap->size = init_heap->size-1;
		
		int v = min->vertex;
		diff[v] = 0;
		nodeEdge* node_curr = edges[v]->next;

		while(node_curr){
			int x = node_curr->vertex;
			if(diff[x] && (node_curr->weight < weights[x])){
				weights[x] = node_curr->weight;
				prev_node[x] = v;
				addNode(x, weights[x], init_heap);
				size = size+1;
			}
			node_curr = node_curr->next;
		}
		count = count + 1;
		free(min);
	}

	float tot_weight = 0.0;
	//float largest_edge_used = 0.0;
	int largest_index = -3;
	for(int i = 0; i < numpoints; i++){
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
*/
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

    /* 
    Initialize set of edges by creating adjacency list and setting
    the weight of each edge to the distance between the coordinates
    (position is given in coordinates)
    */
   /************************ End Set Graph ************************/
}
