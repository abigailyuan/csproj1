//
//  disk.c
//  computer-system-project
//
//  Created by Meng Yuan on 30/3/17.
//  Copyright © 2017 Meng Yuan. All rights reserved.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h> // for open
#include <unistd.h> // for close
#include <stdbool.h>
#include <assert.h>
#include "disk.h"

// da gou
Process push(List *list, Process p){

	if ((*list) == NULL) {
		(*list) = (List)malloc(sizeof(struct list_t));
		(*list)->head = NULL;
		(*list)->tail = NULL;
	}

	Node node = (Node)malloc(sizeof(struct node_t));
	node->data = p;
	node->prev = NULL;
	node->next = NULL;

	if ((*list)->tail == NULL) {
		(*list)->head = node;
		(*list)->tail = node;
	} else {
		(*list)->tail->next = node;
		node->prev = (*list)->tail;
		(*list)->tail = node;
	}

	return p;
}

// da gou
Process pop(List *list){
	if ((*list)->head == NULL) {
		return NULL;
	} else {
		Process data = (*list)->head->data;

		(*list)->head = (*list)->head->next;
		(*list)->tail = (*list)->head == NULL ? NULL : (*list)->tail;

		return data;
	}
}

Process insertBefore(Node node, Process p, List *list){
	Node newNode = (Node)malloc(sizeof(struct node_t));
	newNode->data = p;
	newNode->prev = NULL;
	newNode->next = NULL;

	newNode->next = node;
	newNode->prev = node->prev;
	if(newNode->prev == NULL){
		(*list)->head = newNode;
	}else{
		node->prev->next = newNode;
	}
	node->prev = newNode;
	//printf("new head is %d\n", (*list)->head->data->process_id);
	return newNode->data;
}

Node findHole(List *list, Process p){
	Node hole = NULL;
	Node current = (*list)->head;
	while(current!=NULL){
		if(current->data->timein == -1 && current->data->size >= p->size){
			hole = current;
			break;
		}
		current = current->next;
	}
	return hole;
}

Process removeNode(Node node, List *list){

  if(node->prev == NULL && node->next == NULL){
		(*list)->head = NULL;
		(*list)->tail = NULL;

	}else if(node->prev == NULL && node->next != NULL){
		(*list)->head = node->next;
		node->next->prev = NULL;

	}else if(node->prev != NULL && node->next == NULL){
		(*list)->tail = node->prev;
		node->prev->next = NULL;
	}else if(node->prev != NULL && node->next != NULL){
		node->prev->next = node->next;
		node->next->prev = node->prev;
	}


	// if(node->prev == NULL && node->next == NULL){
	// 	(*list)->head = NULL;
	// 	(*list)->tail = NULL;
	// }else if(node->prev == NULL){
	// 	(*list)->head = node->next;
	// }else if(node->next == NULL){
	// 	(*list)->tail = node->prev;
	// }
	return node->data;
}


Process popLongest(List *list, int time){
	Node node = (*list)->head;
	Node longest = node;
	Process data = node->data;
	int timein = data->time_created;
	while(node != NULL){
		if((time - node->data->time_created) > (time - longest->data->time_created)){
			longest = node;
		}
		if (node->data->time_created == longest->data->time_created) {
			if (node->data->process_id < longest->data->process_id) {
				longest = node;
			}
		}
	}
	data = longest->data;
	// while(node != NULL){
	// 	if(node->data->time_created > data->time_created){
	// 		data = node->data;
	// 		longest = node;
	// 	}
	// 	if(node->data->time_created == data->time_created){
	// 		if(node->data->process_id < data->process_id){
	// 			data = node->data;
	// 			longest = node;
	// 		}
	// 	}
		//node = node->next;

	if(longest->prev == NULL && longest->next == NULL){
		(*list)->head = NULL;
		(*list)->tail = NULL;
	}else if(longest->prev == NULL){
		pop(list);
	}else if(longest->next == NULL){
		(*list)->tail = NULL;
	}
	return data;
}

//pop oldest fromrrq
Process popOldest(List *list){
	Node oldest = (*list)->head;
	Node current = (*list)->head;
	while(current != NULL){
		if(current->data->timein < oldest->data->timein){
			oldest = current;
		}
		current = current->next;
	}
	Process data = oldest->data;
	removeNode(oldest, list);

	return data;
}

Node popFromMemory(List *list, Process p){
	int id = p->process_id;
	Node oldest = (*list)->head;
	Node current = (*list)->head;
	while(current != NULL){
		if(current->data->process_id == id){

			oldest = current;
			break;
		}
		current = current->next;
	}
	Process data = oldest->data;
	//printf("in memory remove\n");
	//removeNode(oldest, list);
	oldest->data->process_id = -1;
	oldest->data->timein = -1;
	return oldest;
}

// da gou
int numOfProcess(Node head){
  int num = 0;
  while(head != NULL){
    num++;
    head = head->next;
  }
  return num;
}
