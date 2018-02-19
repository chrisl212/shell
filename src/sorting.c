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

void _node_enqueue(Node **addr, Node *n) {
	//appends a node to the end of a list
	Node *list = *addr;
	if (!list) {
		//if the list doesn't exist, start it with n
		*addr = n;
		return;
	}
	while (list->next) {
		//loop through to end of list
		list = list->next;
	}
	list->next = n;
	n->next = NULL;
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

void _sort_list(List *lists, double *n_cmp) {
	//sorts each sublist using insertion sort
	while (lists) {
		Node *list = lists->node; 
		if (!list || !list->next) {
			//empty or one element list
			lists = lists->next;
			continue;
		}

		Node *head = NULL; //the sorted list
		while (list != NULL) {
			Node *current = list; //sets the current element to the first in the array
			list = list->next;
			if (!head || current->value < head->value) {
				//if the sorted list is empty or the current value is less than the first, replace the first value in the list
				(*n_cmp)++;
				current->next = head;
				head = current;
			} else {
				Node *p = head; //previous element
				while (p) {
					if (!p->next || current->value < p->next->value) {
						//checks if the next value is less than the key
						(*n_cmp)++;
						current->next = p->next;
						p->next = current;
						break;
					}
					(*n_cmp)++;
					p = p->next; //moves the previous element forward
				}
			}
		}
		lists->node = head; //sets the sublist to the sorted version
		lists = lists->next;
	}
}

Node *Shell_Sort_List(Node *nums, double *n_cmp) {
	long size = nums->value; //fetches the number of numbers
	Node *sz_n = nums;
	nums = nums->next;
	int len = 0; 
	int *seq = _gen_sequence(size, &len); //generates the sequence that will be used for shell sort
	List *lists = NULL;
	List *temp = lists;
	for (int a = len; a >= 0; a--) {
		int k = seq[a]; //fetches the k value
		if (!lists) {
			lists = calloc(1, sizeof(*lists));
			temp = lists;
			for (int i = 1; i < k; i++) {
				//Creates a list of k linked lists, where k is from the sequence
				temp->next = calloc(1, sizeof(*temp));
				temp = temp->next;
			}
		}
		temp = lists;
		for (int i = 1; i < k; i++) {
			temp = temp->next;
		}
		List *remnant = temp->next; //only frees the linked lists when the k value decreases, rather than freeing them at the end of each subsort
		if (remnant) {
			//loops through and frees the unneeded lists
			temp->next = NULL;
			while (remnant) {
				List *t = remnant->next;
				free(remnant);
				remnant = t;
			}
		}
		temp = lists;
		for (int i = 0; i < size; i++) {
			//goes through the list and fills out the k sublists
			if (!temp) {
				temp = lists;
			}
			Node *n = _node_dequeue(&nums); //dequeues from main lists
			_node_enqueue(&(temp->node), n); //enqueues into sublist
			temp = temp->next;
			n = n->next;
		}
		_sort_list(lists, n_cmp); //sorts each list using insertion sort
		temp = lists;
		for (int i = 0; i < size; i++) {
			//goes through the sublists, dequeues each element, and enqueues it back into the main list
			if (!temp) {
				temp = lists;
			}
			Node *n = _node_dequeue(&(temp->node)); //dequeues from sublist
			_node_enqueue(&nums, n); //enqueues into main list
			temp = temp->next;
		}
	}
	free(lists); //frees the last list
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
