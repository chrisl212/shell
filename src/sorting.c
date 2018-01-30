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
	for (int gap = 1; gap < size; gap=(gap*3)+1) {
		for (int i = gap; i < size; i++) {
			int temp = array[i];
			int j;
			for (j = i; j >= gap && array[j-gap] > temp; j -= gap, (*n_cmp)++) {
				array[j] = array[j-gap];
			}
			array[j] = temp;
		}
	}
}

Node *_node_create(long val) {
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

	Node *root = _node_create(0);
	Node *temp = root;
	for (int i = 0; i < size; i++) {
		if (i == 0) {
			root->value = array[i];
		} else {
			temp->next = _node_create(array[i]);
			temp = temp->next;
		}
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
	while (list) {
		bytes += fwrite(&(list->value), sizeof(long), 1, f);
		list = list->next;
	}

	fclose(f);
	return bytes;
}

void Shell_Sort_List(Node *list, double *n_cmp) {

}

