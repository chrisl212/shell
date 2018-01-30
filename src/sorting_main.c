#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "sorting.h"

void _free_node(Node *node) {
	if (node && node->next) {
		_free_node(node->next);
	}
	free(node);
}

int main(int argc, char **argv) {
	if (argc != 4) {
		return EXIT_FAILURE;
	}

	if (strcmp(argv[1], "-a") == 0) {
		int size = 0;
		double n_cmp = 0.0;

		clock_t io = 0;
		clock_t sorting = 0;
		clock_t initial = clock();
		
		long *array = Load_Into_Array(argv[2], &size);
		io += clock()-initial;

		if (!array) {
			return EXIT_FAILURE;
		}

		initial = clock();
		Shell_Sort_Array(array, size, &n_cmp);
		sorting = clock()-initial;

		initial = clock();
		int saved = Save_From_Array(argv[3], array, size);
		io += clock()-initial;
		if (saved < size) {
			return EXIT_FAILURE;
		}

		fprintf(stdout, "I/O time: %le\nSorting time: %le\nNumber of comparisons: %le\n", (double)io, (double)sorting, n_cmp);
		free(array);
		return EXIT_SUCCESS;
	} else if (strcmp(argv[1], "-l") == 0) {
		double n_cmp = 0.0;
		Node *root = Load_Into_List(argv[2]);
		Shell_Sort_List(root, &n_cmp);
		Save_From_List(argv[3], root);
		_free_node(root);
		return EXIT_SUCCESS;
	}

	return EXIT_FAILURE;
}
