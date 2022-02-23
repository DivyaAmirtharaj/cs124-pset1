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
    int* position;
    heapNode** heap_array;
} heap;

/**************** Heap *****************/

// Add a new node to the heap
heapNode* addNode(int vertex, float weight)
{
	heapNode* addNode = (heapNode*)malloc(sizeof(heapNode));
	addNode->vertex = vertex;
	addNode->weight = weight;
	return addNode;
}

// Actually build the min heap
heap* buildMinHeap()
{
	heap* minHeap = (heap*)malloc(sizeof(heap));
	minHeap->position = (int*)malloc(capacity * sizeof(int));
	minHeap->size = 0;
	minHeap->capacity = capacity;
	minHeap->heap_array = (heapNode**)malloc(capacity * sizeof(heapNode*));
	return minHeap;
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

void decrease(heap* minHeap, int vertex, int weight)
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
}

// Pop minimium node
heapNode* popMin(heap* minHeap){
	heapNode* root = minHeap->heap_array[0];
	heapNode* prev = minHeap->heap_array[minHeap->size - 1];
	minHeap->heap_array[0] = prev;
	minHeap->position[root->vertex] = minHeap->size - 1;
	minHeap->position[prev->vertex] = 0;

	// Check heap validity
	--minHeap->size;
	fixHeap(minHeap, 0);

	return root;
}
/************* End of Heap *************/


/**************** Prim's Algorithm *****************/
/*float prim(struct graph* input_graph)
{
	int v = numpoints; //(int v = graph->V) Get the number of vertices in graph
	int parent[v]; // Array to store constructed MST
	int weight[v]; // Key values used to pick minimum weight edge in cut

	// finalHeap represents set E
	heap* finalHeap = buildMinHeap(numpoints);

	// Initialize min heap with all vertices. Key value of
	// all vertices (except 0th vertex) is initially infinite
	for (int v = 1; v < V; ++v) {
		parent[v] = -1;
		weight[v] = INT_MAX;
		finalHeap->heap_array[v] = addNode(v, weight[v]);
		finalHeap->position[v] = v;
	}

	// Make key value of 0th vertex as 0 so that it
	// is extracted first
	weight[0] = 0;
	finalHeap->heap_array[0] = addNode(0, weight[0]);
	finalHeap->position[0] = 0;

	// Initially size of min heap is equal to V
	finalHeap->size = numpoints;

	// In the following loop, min heap contains all nodes
	// not yet added to MST.
	while (finalHeap->size = 0) {
		// Extract the vertex with minimum key value
		heapNode* heapNode = popMin(finalHeap);
		int u = heapNode->v; // Store the extracted vertex number

		// Traverse through all adjacent vertices of u (the extracted
		// vertex) and update their key values
		struct AdjListNode* pCrawl = graph->heap_array[u].head;
		while (pCrawl != NULL) {
			int v = pCrawl->dest;

			// If v is not yet included in MST and weight of u-v is
			// less than key value of v, then update key value and
			// parent of v
			if (isInMinHeap(minHeap, v) && pCrawl->weight < key[v]) {
				key[v] = pCrawl->weight;
				parent[v] = u;
				decreaseKey(minHeap, v, key[v]);
			}
			pCrawl = pCrawl->next;
		}
	}

	// print edges of MST
	printArr(parent, V);
}

 
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
