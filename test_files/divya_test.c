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

typedef struct heap_node{
	int vertex;
	float dist; 
} heap_node;

typedef struct heap {
	int size;
	int capacity;
	heap_node** elements;
} heap;

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
	if(atoi(argv[4]) < 0){
		printf("Dimension is not valid");
		return 1;
	}
	// Set command line inputs
	int flag = atoi(argv[1]);
	int numpoints = atoi(argv[2]);
	int numtrials = atoi(argv[3]);
	int dimension = atoi(argv[4]);

    srand(time(NULL));
	clock_t begin = clock();

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
		printf("Edges from %d: ", i);
		print_lst(edgeArr[i]);
	}
    clock_t end = clock();
	float time_elapsed = (double)(end - begin) / CLOCKS_PER_SEC;
	printf("Avg. weight, numpoints, numtrials, dimension: \n%f, %d, %d, %d\nTime elapsed: %f\n", 
		    (total_weight / (numtrials - failures)), numpoints, numtrials - failures, dimension, time_elapsed);
	return 0;
}
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

