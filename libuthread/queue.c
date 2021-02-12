#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

struct node {
	int *data;
	struct node *next;
}; 

//typedef struct node node;

struct queue {
	int length;
	node *front;
	node *back;
};

queue_t queue_create(void)
{
	queue_t que = malloc(1*sizeof(*que));


	if(que == NULL){
		return NULL;
	}
	
	que->length = 0;
	que->front = NULL;
	que->back = NULL;
	
	return que;
}

// https://stackoverflow.com/questions/6417158/c-how-to-free-nodes-in-the-linked-list
int queue_destroy(queue_t queue)
{
	if(queue == NULL || queue->length != 0){
		return -1;
	}

	free(queue);
	queue = NULL;

	return 0;
}

//https://www.codesdope.com/blog/article/making-a-queue-using-linked-list-in-c/
int queue_enqueue(queue_t queue, void *data)
{
	if(queue == NULL || data == NULL){
		return -1;
	}

	struct node* new = malloc(1*sizeof(node));
	if (new == NULL){
		return -1;
	}

	new->data = data;
	new->next = NULL;

	if(queue->length == 0){
		queue->front = new;
		queue->back = new;
	} else {
		queue->back->next = new;
		queue->back = new;
	}

	queue->length++;

	return 0;
}

int queue_dequeue(queue_t queue, void **data){

	if(queue == NULL || data == NULL || queue->length == 0){
		return -1;
	}

	*data = queue->front->data;
	queue->front = queue->front->next;
	queue->length--;

	return 0;
}

int queue_delete(queue_t queue, void *data){

	if(queue == NULL || data == NULL){
		return -1;
	}

	int del = 0;

	struct node *current = queue->front; 
	struct node *prev = current;
	
	while(current != NULL && del != 1){
	
		if (current->data == data){
			prev->next = current->next;
			if (queue->front == current) {
				queue->front = current->next;
			}
			del = 1;
			queue->length--;
		}else{
			prev = current;
			current = current->next; 
		}
	}

	if (del == 1) {
		return 0;
	} else {
		return -1;
	}
	
}

int queue_iterate(queue_t queue, queue_func_t func, void *arg, void **data)
{
	if(queue == NULL || func == NULL){
		return -1;
	}

	int retval = 0;

	node *current = queue->front;
	node *next = current->next;

	while(current != NULL && retval != 1){
		next = current->next;
		retval = func(queue, current->data, arg);

		if(next != current->next){
			next = current->next;
		} 

		if (data != NULL && retval == 1){
			*data = current->data;
		}

		current = next;
	}

	return 0;
}

int queue_length(queue_t queue)
{
	if(queue == NULL){
		return -1;
	} 

	return queue->length;
}


// void queue_print(node* head){
	// if(head == NULL){
		// printf("NULL\n");
	// }else{
		// printf("%d --> ", head->data);
		// queue_print(head->next);
	// }
// }
//  