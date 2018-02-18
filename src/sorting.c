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

	Node *root = _node_create(size); 
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

void _node_push(Node **addr, Node *n) {
	Node *list = *addr; //fetches the list from the address
	if (!list) {
		//if the list doesn't exist, create it 
		*addr = n;
		return;
	}
	if (!list->next) {
		//if there is only one node, set the next to n
		list->next = n;
		return;
	}
	while (list->next->next) {
		//loop through until at penultimate node
		list = list->next;
	}
	list->next->next = n; //set last node to n
}

void _node_enqueue(Node **addr, Node *n, double *n_cmp) {
	Node *list = *addr; //fetches list from the address
	if (!list) {
		//if the list doens't exists creates it
		*addr = n;
		return;
	}
	if (n->value < list->value) {
		//if the value is smaller than the first sets it to the first
		if (n_cmp) {
			(*n_cmp)++;
		}
		*addr = n;
		n->next = list;
	} else if (!list->next) {
		//if there is only one node sets n to the next one
		list->next = n;
		(*n_cmp)++;
	} else {
		while (list->next && n->value > list->next->value) {
			//loops through the list until at end or n is smaller than the next node
			if (n_cmp) {
				(*n_cmp)++;
			}
			list = list->next;
		}
		(*n_cmp) += 2; //increments counter by two due to comparisons in while and if that are unaccounted for
		Node *temp = list->next;
		list->next = n; //inserts n in the list
		n->next = temp;
	}
}

Node *_node_dequeue(Node **nums) {
	if (!*nums) {
		return NULL;
	}
	Node *temp = *nums; //dequeues the first node from the list
	*nums = (*nums)->next;
	temp->next = NULL;
	return temp;
}

Node *Shell_Sort_List(Node *nums, double *n_cmp) {
	long size = nums->value; //fetches the number of numbers
	Node *sz_n = nums;
	nums = nums->next;
	int len = 0; 
	int *seq = _gen_sequence(size, &len); //generates the sequence that will be used for shell sort
	for (int a = len; a >= 0; a--) {
		int gap = seq[a]; //fetches the k value
		List *lists = calloc(1, sizeof(*lists)); //a list of linked lists
		for (int i = 0; i < gap; i++) {
			List *temp = lists;
			//fills out the list of linked lists
			while (temp->next) {
				temp=temp->next;
			}
			temp->next = calloc(1, sizeof(*temp));
		}
		List *temp = lists; 
		for (int i = 0; i < size; i++) {
			if (!temp) {
				temp = lists;
			}
			Node *n = _node_dequeue(&nums); //dequeues a node from the number list
			if (!lists->node) {
				//if the list is empty, set the first item to n
				lists->node = n;
			} else {
				//enqueues a node into the sorted sublist
				_node_enqueue(&(lists->node), n, n_cmp);
			}
			temp = temp->next;
		}
		temp = lists;
		for (int i = 0; i < size; i++) {
			if (!temp) {
				temp = lists;
			}
			Node *n = _node_dequeue(&(lists->node)); //dequeues the first node of the sublist
			_node_push(&nums, n); //pushes that node onto the main list
			temp = temp->next;
		}
		while (lists) {
			temp = lists->next;
			free(lists);
			lists = temp;
		}
	}
	sz_n->next = nums; //puts the node containing the size back at the front of the list

	free(seq);
	return sz_n;
}

int Save_From_List(char *filename, Node *list) {
	FILE *f = fopen(filename, "w"); //opens the file for writing 
	if (!f) {
		//if the file cannot be opened, returns EXIT_FAILURE
		return EXIT_FAILURE;
	}

	int bytes = 0;
	list = list->next;
	while (list) {
		//writes each integer to the file
		bytes += fwrite(&(list->value), sizeof(long), 1, f);
		list = list->next;
	}

	fclose(f);
	return bytes;
}
