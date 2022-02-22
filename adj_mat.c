#include <stdio.h>
#include <stdlib.h>

// Define the maximum number of vertices in the graph
#define N 6

// Data structure to store a graph object
typedef struct Graph
{
	struct Node* head[N];
} Graph;

// Data structure to store adjacency list nodes of the graph
typedef struct nodeEdge{
    int end;
    float weight;
    struct nodeEdge* next;
} nodeEdge;

// Data structure to store a graph edge
typedef struct edge {
	int start;
    int end;
    float weight;
} edge;

// Function to create an adjacency list from specified edges
Graph* createGraph(edge edges[], int n)
{
	// allocate storage for the graph data structure
	Graph* graph = (Graph*)malloc(sizeof(Graph));

	// initialize head pointer for all vertices
	for (int i = 0; i < N; i++) {
		graph->head[i] = NULL;
	}

	// add edges to the directed graph one by one
	for (int i = 0; i < n; i++)
	{
		// get the source and destination vertex
		int start = edges[i].start;
		int end = edges[i].end;
		float weight = edges[i].weight;

		// allocate a new node of adjacency list from src to dest
		nodeEdge* newNode = (nodeEdge*)malloc(sizeof(nodeEdge));
		newNode->end = end;
		newNode->weight = weight;

		// point new node to the current head
		newNode->next = graph->head[start];

		// point head pointer to the new node
		graph->head[start] = newNode;
	}

	return graph;
}

// Function to print adjacency list representation of a graph
void printGraph(Graph* graph)
{
	for (int i = 0; i < N; i++)
	{
		// print current vertex and all its neighbors
		nodeEdge* ptr = graph->head[i];
		while (ptr != NULL)
		{
			printf("%d —> %d (%f)\t", i, ptr->end, ptr->weight);
			ptr = ptr->next;
		}

		printf("\n");
	}
}

// Weighted Directed graph implementation in C
int main(void)
{
	// input array containing edges of the graph (as per the above diagram)
	// (x, y, w) tuple represents an edge from x to y having weight `w`
	edge edges[] =
	{
		{0, 1, 6}, {1, 2, 7}, {2, 0, 5}, {2, 1, 4}, {3, 2, 10}, {4, 5, 1}, {5, 4, 3}
	};

	// calculate the total number of edges
	int n = sizeof(edges)/sizeof(edges[0]);

	// construct a graph from the given edges
	Graph *graph = createGraph(edges, n);

	// Function to print adjacency list representation of a graph
	printGraph(graph);

	return 0;
}