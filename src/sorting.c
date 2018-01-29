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
	return bytes/sizeof(*array);
}

void Shell_Sort_Array(long *array, int size, double *n_cmp) {

}

Node *_node_create(long val) {
	Node *node = malloc(sizeof(*node));
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
	long *array = malloc(size);
	fread(array, sizeof(*array), size, f);

	Node *root = _node_create(0);
	Node *temp = root;
	for (int i = 0; i < size; i++) {
		if (!temp) {
			temp = _node_create(array[i]);
		}
		temp = temp->next;
	}


	return root;
}

int Save_From_List(char *filename, Node *list) {

}

void Shell_Sort_List(Node *list, double *n_cmp) {

}

