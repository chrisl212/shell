#ifndef SORTING_H
#define SORTING_H

typedef struct _Node {
	long value;
	struct _Node *next;
} Node;

typedef struct _List {
	Node *node;
	struct _List *next;
} List;

long *Load_Into_Array(char *filename, int *size);
int Save_From_Array(char *filename, long *array, int size);
void Shell_Sort_Array(long *array, int size, double *n_cmp);
Node *Load_Into_List(char *filename);
int Save_From_List(char *filename, Node *list);
Node *Shell_Sort_List(Node *list, double *n_cmp);

#endif

