#include <stdio.h>
#include <stdlib.h>
#include "sorting.h"

long *Load_Into_Array(char *filename, int *size) {
	FILE *f = fopen(filename, "r"); //open the file for reading
	if (!f) {
		//if the file does not open, return NULL
		return NULL;
	}
	
	fseek(f, 0, SEEK_END);
	long fsize = ftell(f);
	fseek(f, 0, SEEK_SET); //determine the file size
	*size = fsize/sizeof(fsize); //determine the number of longs in the file
	
	long *array = malloc(*size*sizeof(*array)); //create an array to hold the numbers
	fread(array, sizeof(*array), *size, f); //read the numbers into the array
	
	fclose(f);
	return array;
}

int Save_From_Array(char *filename, long *array, int size) {
	FILE *f = fopen(filename, "w"); //opens the file for writing
	if (!f) {
		//if the file does not open, return failure code
		return EXIT_FAILURE;
	}

	int written = fwrite(array, sizeof(*array), size, f); //write the array
	
	fclose(f);
	return written;
}

int *_gen_sequence(int size, int *len) {
	//generates the sequence using 3*h(i-1)+1
	int *array = malloc(sizeof(*array)); //allocates the sequence
	int arr_sz = 1;
	int i = 0;
	array[0] = 1; //sets the first term to 1
	while (3*array[i]+1 < size) {
		//iterates until the number is greater than the size
		//reallocates the array every time it is necessary
		if (++i >= arr_sz) {
			arr_sz *= 2;
			array = realloc(array, arr_sz*sizeof(*array));
		}	
		array[i] = 3*array[i-1]+1;
	}

	*len = i;
	return array;
}

void Shell_Sort_Array(long *array, int size, double *n_cmp) {
	int len = 0; 
	int *seq = _gen_sequence(size, &len); //generates the shell sort sequence
	for (int a = len; a >= 0; a--) {
		int gap = seq[a]; //iterates through all values of k
		for (int j = gap; j < size; j++) {
			//performs insertion sort on the subarrays
			int temp = array[j];
			int i = j;
			while (i >= gap && array[i-gap] > temp) {
				(*n_cmp)++;
				array[i] = array[i-gap]; //moves the value to the sorted array
				i -= gap;
			}
			array[i] = temp;
		}
	}
	free(seq);
}

static Node *_node_create(long val) {
	//creates a node with a given value
	Node *node = calloc(1, sizeof(*node));
	node->value = val;
	return node;
}

Node *Load_Into_List(char *filename) {
	FILE *f = fopen(filename, "r"); //opens the file for reading
	if (!f) {
		//if the file cannot be opened, returns NULL
		return NULL;
	}
	
	fseek(f, 0, SEEK_END);
	long fsize = ftell(f); //determine the size of the file
	fseek(f, 0, SEEK_SET);

	long size = fsize/sizeof(fsize); //counts the number of integers
	long *array = malloc(size*sizeof(*array)); //allocates the array
	fread(array, sizeof(*array), size, f); //reads in the numbers

	Node *root = _node_create(size); //the first node is a dummy that contains the size of the array
	Node *temp = root;
	for (int i = 0; i < size; i++) {
		//creates a node for each integer 
		temp->next = _node_create(array[i]);
		temp = temp->next;
	}

	free(array);
	fclose(f);
	return root;
}

int Save_From_List(char *filename, Node *list) {
	FILE *f = fopen(filename, "w"); //opens the file for writing 
	if (!f) {
		//if the file cannot be opened, returns EXIT_FAILURE
		return EXIT_FAILURE;
	}

	int bytes = 0;
	list=list->next;
	while (list) {
		//writes each integer to the file
		bytes += fwrite(&(list->value), sizeof(long), 1, f);
		list = list->next;
	}

	fclose(f);
	return bytes;
}

void _swap(Node **array, int i, int j) {
	//swaps the nodes at i and j in an array
	//sets the previous nodes' next values to the correct nodes
	if (i > 0) {
		array[i-1]->next = array[j];
	}
	if (j > 0) {
		array[j-1]->next = array[i];
	}
	Node *temp = array[i]->next;
	array[i]->next = array[j]->next; //sets the swapped nodes next values
	array[j]->next = temp;
	temp = array[i];
	array[i] = array[j]; //swaps the pointers
	array[j] = temp;
}

Node *Shell_Sort_List(Node *list, double *n_cmp) {
	int size = list->value; //gets the size of the array from the first node
	Node *n = list->next;
	Node **array = malloc(size*sizeof(*array)); //creates an array to hold the nodes
	for (int i = 0; n != NULL; i++, n=n->next) {
		//assigns each node to the array
		array[i] = n;
	}

	int len = 0; 
	int *seq = _gen_sequence(size, &len); //generates the shell sort sequence
	for (int a = len; a >= 0; a--) {
		int gap = seq[a]; //iterates through each k value in the sequence
		for (int j = gap; j < size; j++) {
			//performs insertion sort on each subarray
			for (int i = j; i >= gap; i-=gap) {
				(*n_cmp)++;
				if (array[i-gap]->value > array[i]->value) {
					//swaps the values
					_swap(array, i-gap, i);
				}
				else {
					break;
				}
			}
		}
	}
	if (size > 0) {
		list->next = array[0]; //sets the first node of the list to the first element of the array, as it has likely changed
	}

	free(array);
	free(seq);

	return list;
}

