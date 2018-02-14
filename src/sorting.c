#include <stdio.h>
#include <stdlib.h>
#include "sorting.h"

long *Load_Into_Array(char *filename, int *size) {
	FILE *f = fopen(filename, "r");
	if (!f) {
		return NULL;
	}
	
	fseek(f, 0, SEEK_END);
	long fsize = ftell(f);
	fseek(f, 0, SEEK_SET);
	*size = fsize/sizeof(fsize);
	
	long *array = malloc(*size*sizeof(*array));
	fread(array, sizeof(*array), *size, f);
	
	fclose(f);
	return array;
}

int Save_From_Array(char *filename, long *array, int size) {
	FILE *f = fopen(filename, "w");
	if (!f) {
		return -1;
	}

	int bytes = fwrite(array, sizeof(*array), size, f);
	
	fclose(f);
	return bytes;
}

void Shell_Sort_Array(long *array, int size, double *n_cmp) {
	int max = 1;
	while (max*3+1 < size) {
		max = max*3+1;
	}
	for (int gap = max; gap > 0; gap=(gap-1)/3) {
		for (int j = gap; j < size; j++) {
			int temp = array[j];
			int i = j;
			while (i >= gap && array[i-gap] > temp) {
				(*n_cmp)++;
				array[i] = array[i-gap];
				i -= gap;
			}
			array[i] = temp;
		}
	}
}

static Node *_node_create(long val) {
	Node *node = calloc(1, sizeof(*node));
	node->value = val;
	return node;
}

Node *Load_Into_List(char *filename) {
	FILE *f = fopen(filename, "r");
	if (!f) {
		return NULL;
	}
	
	fseek(f, 0, SEEK_END);
	long fsize = ftell(f);
	fseek(f, 0, SEEK_SET);

	long size = fsize/sizeof(fsize);
	long *array = malloc(size*sizeof(*array));
	fread(array, sizeof(*array), size, f);

	Node *root = _node_create(size);
	Node *temp = root;
	for (int i = 0; i < size; i++) {
		temp->next = _node_create(array[i]);
		temp = temp->next;
	}

	free(array);
	fclose(f);
	return root;
}

int Save_From_List(char *filename, Node *list) {
	FILE *f = fopen(filename, "w");
	if (!f) {
		return -1;
	}

	int bytes = 0;
	list=list->next;
	while (list) {
		bytes += fwrite(&(list->value), sizeof(long), 1, f);
		list = list->next;
	}

	fclose(f);
	return bytes;
}

void _swap(Node **array, int i, int j) { 
	if (i > 0) {
		array[i-1]->next = array[j];
	}
	if (j > 0) {
		array[j-1]->next = array[i];
	}
	Node *temp = array[i]->next;
	array[i]->next = array[j]->next;
	array[j]->next = temp;
	temp = array[i];
	array[i] = array[j];
	array[j] = temp;
}

void Shell_Sort_List(Node *list, double *n_cmp) {
	int max = 1;
	int size = list->value;
	Node *n = list->next;
	Node **array = malloc(size*sizeof(*array));
	for (int i = 0; n != NULL; i++, n=n->next) {
		array[i] = n;
	}

	while (max*3+1 < size) {
		max = max*3+1;
	}
	for (int gap = max; gap > 0; gap=(gap-1)/3) {
		for (int j = gap; j < size; j++) {
			for (int i = j; i >= gap; i-=gap) {
				(*n_cmp)++;
				if (array[i-gap]->value > array[i]->value) {
					_swap(array, i-gap, i);
				}
				else {
					break;
				}
			}
		}
	}
	list->next = array[0];

	free(array);
}

