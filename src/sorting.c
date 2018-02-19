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

Node *_node_dequeue(Node **addr) {
	//dequeues a node from the beginning of a list
	Node *list = *addr;
	if (!list) {
		//if the list doesn't exist, stop
		return NULL;
	}
	*addr = list->next;
	list->next = NULL;
	return list;
}

void _node_enqueue(Node **addr, Node *n) {
	//inserts the node at the beginning of the list at addr
	Node *list = *addr;
	if (!list) {
		//if the list doesn't exist, create it with n
		*addr = n;
		return;
	}
	*addr = n;
	n->next = list;
}

void _node_enqueue_o(Node **addr, Node *n, double *n_cmp) {
	//inserts the node at the correct position for the list to be in descending order
	//insertion sort - a sorted list is created separately from the values in the main list
	//it is sorted in descending order so that nodes can be enqueued into the main list in the reverse order later - see Shell_Sort_List
	Node *list = *addr;
	if (!list) {
		//if the list doesnt' exist, create it with n
		*addr = n;
		return;
	}
	if (++(*n_cmp) && n->value > list->value) {
		//if the new node is greater than the first node, insert it at position 0
		n->next = list;
		*addr = n;
		return;
	}
	while (list->next && (++(*n_cmp) && n->value < list->next->value)) {
		//loop until a node smaller than n is reached
		list = list->next;
	}
	n->next = list->next; //insert n into its proper place
	list->next = n;
}

Node *Shell_Sort_List(Node *nums, double *n_cmp) {
	long size = nums->value; //fetches the number of numbers
	Node *sz_n = nums;
	nums = nums->next;
	int len = 0; 
	int *seq = _gen_sequence(size, &len); //generates the sequence that will be used for shell sort
	List *temp = NULL;
	List *lists = calloc(1, sizeof(*lists));
	temp = lists;
	for (int a = len; a >= 0; a--) {
		int k = seq[a]; //fetches the k value
		temp = lists;
		for (int i = 0, l = 0; i < size; i++) {
			//goes through the list and fills out the k sublists
			Node *n = _node_dequeue(&nums); //dequeues from main lists
			_node_enqueue_o(&(temp->node), n, n_cmp); //inserts into sublist such that the list is in descending order
			if (++l >= k) {
				temp = lists;
			} else if (temp->next) {
				temp = temp->next;
			} else {
				temp->next = calloc(1, sizeof(*temp));
				temp = temp->next;
			}
		}
		for (int i = 0, l = 0; i < size; i++, l++) {
			//goes through the sublists, dequeues each element, and enqueues it back into the main list
			if (l >= k) {
				temp = lists;
			}
			Node *n = _node_dequeue(&(temp->node)); //dequeues from sublist
			_node_enqueue(&nums, n); //enqueues into main list, inserting at front- this reverses the descending order from earlier and allows insertion without traversing the list
			temp = temp->next;
		}
	}
	while (lists) {
		temp = lists->next;
		free(lists);
		lists = temp;
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
	//_print_list(list);
	while (list) {
		//writes each integer to the file
		bytes += fwrite(&(list->value), sizeof(long), 1, f);
		list = list->next;
	}

	fclose(f);
	return bytes;
}
