#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "sorting.h"

static void _free_node(Node *node) {
	//frees a node and all its connected nodes
	Node *temp = NULL;
	while (node) {
		//uses a while loop instead of recursion for memory purposes
		temp = node;
		node = node->next;
		free(temp);
	}
}

int main(int argc, char **argv) {
	if (argc != 4) {
		//requires 4 arguments
		return EXIT_FAILURE;
	}
	if (strcmp(argv[1], "-a") && strcmp(argv[1], "-l")) {
		//invalid option
		return EXIT_FAILURE;
	}
	int size = 0; //number of integers
	double n_cmp = 0.0; //number of sorting comparisons

	clock_t io = 0; //I/O time
	clock_t sorting = 0; //sorting time
	clock_t initial = clock(); //initial clock time

	long *array = NULL;
	Node *root = NULL;
	if (strcmp(argv[1], "-a") == 0) {
		//if -a, loads integers into array
		array = Load_Into_Array(argv[2], &size);
		if (!array) {
			return EXIT_FAILURE;
		}
	} else {
		//if -l, loads integers into linked list
		root = Load_Into_List(argv[2]);
		if (!root) {
			return EXIT_FAILURE;
		}
	}
	io += clock()-initial;

	initial = clock();
	//performs shell sort on the integers
	if (array) {
		Shell_Sort_Array(array, size, &n_cmp);
	} else {
		Shell_Sort_List(root, &n_cmp);
	}
	sorting = clock()-initial;

	initial = clock();
	//saves the sorted integers to a file
	if (array) {
		Save_From_Array(argv[3], array, size);
	} else {
		Save_From_List(argv[3], root);
	}
	io += clock()-initial;

	fprintf(stdout, "I/O time: %le\nSorting time: %le\nNumber of comparisons: %le\n", (double)io/CLOCKS_PER_SEC, (double)sorting/CLOCKS_PER_SEC, n_cmp);
	if (array) {
		free(array);
	} else {
		_free_node(root);
	}
	
	return EXIT_SUCCESS;
}
