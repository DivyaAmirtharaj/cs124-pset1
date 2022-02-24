#include <limits.h> 
#include <stdbool.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <float.h>
#include <math.h>
#include <time.h>
#include <sys/resource.h>

typedef struct heap {
    int label;
    float weight;
} heap;


void heapify(heap h[], int i, int size, int *pos)
{
    heap temp;

    int smallest, left, right;
    left = 2 * i + 1;
    right = left + 1;
    smallest = i;

    if (left >= size)
        return;

    else {
        if (left < size && h[left].weight < h[i].weight) 
            smallest = left;

        if (right < size && h[right].weight < h[smallest].weight)  
            smallest = right;

        if (smallest != i) {
            temp = h[i];
            h[i] = h[smallest];
            h[smallest] = temp;

            int tempind = pos[h[i].label];
            pos[h[i].label] = pos[h[smallest].label];
            pos[h[smallest].label] = tempind;

            heapify(h, smallest, size, pos);
        }
    }
}

heap delete_min(heap h[], int *size, int *pos){
    // pull smallest value
    heap min = h[0];
    pos[h[0].label] = -1;

    // replace first element with last element 
    h[0] = h[*size - 1];
    pos[h[0].label] = 0;
    
    // shift pointer
    *size = *size - 1;

    heapify(h, 0, *size, pos);
    return min;
}

void fix(heap h[], int i, int *pos) {
    if (h[i].weight < h[i / 2].weight) {
        heap temp = h[i / 2];
        h[i / 2] = h[i];
        h[i] = temp;

        int tempind = pos[h[i].label];
        pos[h[i].label] = pos[h[i / 2].label];
        pos[h[i / 2].label] = tempind;
        fix(h, i / 2, pos);
    }
}

int main(int argc, char *argv[]) {

	if(argc != 5){
		printf("Please add inputs: flag, numpoints, numtrials, dimension");
		return 1;
	}

	// Set command line inputs
	int flag = atoi(argv[1]);
	int numpoints = atoi(argv[2]);
	int numtrials = atoi(argv[3]);
	int dimension = atoi(argv[4]);

}
