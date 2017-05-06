//
//  disk.h
//  computer-system-project
//
//  Created by Meng Yuan on 30/3/17.
//  Copyright © 2017 Meng Yuan. All rights reserved.
//

#ifndef disk_h
#define disk_h


#include        <stdio.h>
#include        <stdlib.h>
#include        <string.h>



typedef struct process_t* Process;

struct process_t {
    int process_id;
    int job_time;
    int time_created;
    int size;
    int timein;
};


typedef struct node_t* Node;
struct node_t {
	Process data;
	Node prev;
	Node next;
};

typedef struct list_t* List;
struct list_t {
	Node head;
	Node tail;
};

//push a process to disk and return the tail of the disk
extern Process push(List *list, Process p);

//pop the head of the disk and return the new head
extern Process pop(List *list);

extern int numOfProcess(Node head);

extern Process insertBefore(Node node, Process p, List *list);

extern Process popLongest(List *list, int time);

extern Process popOldest(List *list);

extern Node popFromMemory(List *list, Process p);

Node findHole(List *list, Process p);

#endif /* disk_h */
