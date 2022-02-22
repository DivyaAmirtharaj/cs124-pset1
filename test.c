/* 
Minimum Spanning Tree Algorithms 
Prim's Algorithm

Implementation:
Maintain two sets of vertices, visited and unvisited
and at each step find the lowest edge weight between
the two sets and select it, and add the corresponding 
vertex to the visited set.


- Adjacency list to store edges and weights

Optimization:
Introduce some upper limit on edge weight such that we 
guarantee that Prim's won't search a weight that large
so we can simply omit them from the heap
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <limits.h>

typedef struct point{
	int dim;
	float* xs;
} point;

typedef struct node {
	int vertex; 
	float dist; 
	struct node* next;  
} node; 

typedef struct heap_node{
	int vertex;
	float dist; 
} heap_node;

typedef struct heap {
	int size;
	int capacity;
	heap_node** elements;
} heap;


// Edges will be stored as (unordered) adjacency lists
node* deleteMinQueue(node* root);
node*  insertLinkedList(int vertex, float distance, node* root);
node* createLinkedList();
node* ListFind(int vertex, node* root);

// MST Calculation
float MSTfind(int numpoints, node* edges[numpoints]);
float dist(point p1, point p2);
void test(void);
float largest_edge = 0;
int flag;
int numtrials;
int failures; 

// Heap Management
int getLeftChild(int x);
int getRightChild(int x);
int getParent(int x);
heap* createHeap();
void insertHeap(int vertex, float distance, heap* root);
heap_node* deleteHeapNode(heap* root);
void fix(heap *root, int i);

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
		
	srand(time(NULL));
	clock_t begin = clock();
    
	// Create an array of adjacency lists to store the edge lengths for each vertex 
    // where edges is the length of number of points
	node* edges[numpoints];
	point points[numpoints];
	for(int i = 0; i < numpoints; i++){
		edges[i] = createLinkedList();
	}

    // initializes random points
	int count = 0;
	float total_weight = 0.0;
	failures = 0;
	while(count < numtrials){
        for(int i = 0; i < numpoints; i++){
            float* coordinates = malloc(sizeof(float)*dimension);
            for(int j = 0; j < dimension; j++){
                coordinates[j] = (float)rand() / (float)RAND_MAX;
            }
            points[i].dim = dimension;
            points[i].xs = coordinates;	
        }

        // getting the distance between points (edge weight)
		for(int i = 0; i < numpoints; i++){
			for(int j = 0; j < numpoints; j++){
				printf("%d, %d\n", i, j);
				float tmp_dst = 0.0;
				node* tmp_node;
				if(i == j){
					tmp_dst = INT_MAX;
					printf("%f\n", tmp_dst);
				}
				else{
					tmp_dst = dist(points[i], points[j]);
					printf("%f\n", tmp_dst);
				}

                insertLinkedList(j, tmp_dst, edges[i]);
                insertLinkedList(i, tmp_dst, edges[j]);

			}
		}

		// Free points
		if(dimension > 0){
			for(int i = 0; i < numpoints; i++){
				free(points[i].xs);
			}
		}

		float weight = MSTfind(numpoints, edges);

		// In case an MST could not be found
		if(weight > 0){
			total_weight += weight;
			//printf("%f, %d, %d\n", weight, numpoints, dimension);
		}

		for(int i = 0; i < numpoints; i++){
			while(edges[i]->next != NULL){
				free(deleteMinQueue(edges[i]));
			}
		}

		count += 1;
	}
	for(int i = 0; i < numpoints; i++){
		free(edges[i]);
	}
	clock_t end = clock();
	float time_elapsed = (double)(end - begin) / CLOCKS_PER_SEC;
	printf("Avg. weight, numpoints, numtrials, dimension: \n%f, %d, %d, %d\nTime elapsed: %f\n", 
		    (total_weight / (numtrials - failures)), numpoints, numtrials - failures, dimension, time_elapsed);
	return 0;

}
/******************************* MST Management ******************************/
float dist(point p1, point p2){
	int n = p1.dim; 
	float distance = 0.0;
	for(int i = 0; i < n; i++){
		distance += (p1.xs[i] - p2.xs[i]) * (p1.xs[i] - p2.xs[i]); 
	}
	return sqrt(distance);
}

float MSTfind(int numpoints, node* edges[numpoints]){
	heap* root = createHeap();//createQueue();
	insertHeap(0,0.0,root);

	float dist[numpoints];
	int prev[numpoints];
	uint8_t V_minus_S[numpoints];

	// Initialize arrays.  
	for(int i = 0; i < numpoints; i++){
		dist[i] = INT_MAX;
		prev[i] = INT_MIN;
		V_minus_S[i] = 1;
	}
	prev[0] = 0;
	dist[0] = 0;
	insertLinkedList(0,0.0,edges[0]);

	int count = 0;
	int size = 1;
	while(root->size > 0){
		heap_node* min_node = deleteHeapNode(root);
		size--;
		
		int v = min_node->vertex;
		V_minus_S[v] = 0;
		node* current = edges[v]->next;

		while(current != NULL){
			int w = current->vertex;
			if(V_minus_S[w] && (current->dist < dist[w])){
				dist[w] = current->dist;
				prev[w] = v;
				insertHeap(w, dist[w], root);
				size++;
			}
			current = current -> next;
		}
		count++;
		if(flag > 1.0){
			if(count % 1000 == 0)
				printf("%d, %d\n", count, size);
		}
		free(min_node);
	}

	float tree_weight = 0.0;
	float largest_edge_used = 0.0;
	int largest_index = -3;
	for(int i = 0; i < numpoints; i++){
		if (prev[i] < 0){
			printf("Vertex %d was not part of the MST\n", i);
			free(root);
			failures++;
			return -1;
		}

		node* temp = ListFind(prev[i], edges[i]);
		float tmp_dst = temp->dist;

		tree_weight += tmp_dst;

		if (tmp_dst > largest_edge_used){
			largest_edge_used = tmp_dst;
			largest_index = i;
		}
	}

	free(root);
	
	largest_edge += largest_edge_used;
	if (flag > 0)
		printf("Largest edge: %f\n", largest_edge_used);
	return tree_weight;
}

/***************************** Queue Management ******************************/

node* deleteMinQueue(node* root){
	if(root->next != NULL){
		node* minNode = root->next;
		root->next = minNode->next;
		return minNode;
	}
	else{
		return NULL;
	}
}

/************************* Adjacency List Management *************************/
node* createLinkedList(){
	node* root = malloc(sizeof(node));
	root->next = NULL;
	root->dist = INT_MIN;
	return root;
}

node* insertLinkedList(int vertex, float distance, node* root){
	node* new_node = malloc(sizeof(node));
	new_node->vertex = vertex;
	new_node->dist = distance;
	new_node->next = root->next;
	root->next = new_node;
	return new_node;
}

node* ListFind(int vertex, node* root){
	node* current = root->next;
	while(current != NULL && current->vertex != vertex){
		current = current->next;
	}
	return current;
}

/****************************** Heap Management ******************************/
int getLeftChild(int x){
	return 2 * x + 1;
}
int getRightChild(int x){
	return 2 * x + 2;
}
int getParent(int x){
	return (x - 1) / 2;
}

heap* createHeap(){
	heap* new_heap = malloc(sizeof(heap)); 
	new_heap->size = 0;
	new_heap->capacity = 0;
	return new_heap;
}

void insertHeap(int vertex, float distance, heap* root){
	// If heap is full, double the amount of space it has
	if(root->capacity == root->size && root->capacity != 0){
		root->elements = realloc(root->elements, (root->capacity * 2) * sizeof(heap_node));
		root->capacity *= 2;
		root->size += 1;
	}
	else if(root->size == 0){
		root->elements = malloc(sizeof(heap_node));
		root->size += 1;
		root->capacity += 1;
	}
	else{
		root->size++;
	}

	heap_node* new_node = malloc(sizeof(heap_node));
	new_node->vertex = vertex;
	new_node->dist = distance;

	int i = (root->size) - 1;
	while(i > 0 && new_node->dist < root->elements[getParent(i)]->dist){
		root->elements[i] = root->elements[getParent(i)]; 
		i = getParent(i);
	}

	root->elements[i] = new_node;
}


heap_node* deleteHeapNode(heap* root){
	if(root->size > 0){
		heap_node* min_node = root->elements[0];
		root->size--;
		root->elements[0] = root->elements[root->size];
		fix(root, 0);
		
		if(root->size <= root->capacity / 2){
			root->elements = realloc(root->elements, (root->capacity / 2) * sizeof(heap_node));
			root->capacity /= 2;
		}
		return min_node;
	}
	else{
		free(root->elements);
		return NULL;
	}
}

void fix(heap *root, int i){
	int left = getLeftChild(i);
	int right = getRightChild(i);
	int smallest = i;


	if(left < root->size && root->elements[left]->dist < root->elements[smallest]->dist){
		smallest = left;
	}
	if(right < root->size && root->elements[right]->dist < root->elements[smallest]->dist){
		smallest = right; 
	}


	if(smallest != i){
		heap_node* temp = root->elements[i];
		root->elements[i] = root->elements[smallest];
		root->elements[smallest] = temp;
		fix(root, smallest);
	}
}
