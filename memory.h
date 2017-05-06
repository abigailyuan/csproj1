//
//  memory.h
//  computer-system-project
//
//  Created by Meng Yuan on 30/3/17.
//  Copyright © 2017 Meng Yuan. All rights reserved.
//

#ifndef memory_h
#define memory_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h> // for open
#include <unistd.h> // for close
#include <assert.h>
#include "disk.h"

typedef struct segmentation_t* Segmentation;
typedef struct freeList_t* FreeList;


struct segmentation_t {
    Segmentation next;
    Process p;
    int size;
    Segmentation prev;
};

struct freeList_t {
  Segmentation* s;
  int size;
  FreeList next;
  FreeList prev;
};





//add a process to memory
extern bool pushToMemory(Process p, int* time, int* memsize, int* usedSize, Segmentation s);

//swap out a process from memory
extern Segmentation popFromMemory(Segmentation head);


//findPlace in freeList
//extern Segmentation findPlace(FreeList freeList, int size, FreeList* head, FreeList* tail);
extern Segmentation findPlace(FreeList* head, FreeList* fTail, int size);

//get the one that needs to be swapped out from rrq
extern Process nextOutFromRRQ(Process rrq);

//create rrq
extern Process createRRQ();

//num of process in rrq
extern int numOfPinRRQ(Process rrqHead);

// remove from rrq
extern void removeFromRRQ(Process p, Process* rrqHead);

//add to rrq
extern void pushToRRQ(Process p, Process rrq);

//swap out a process fromn memory to disk
extern Process swapOut(Segmentation head, Segmentation s);

extern Segmentation createNewSeg(int size);





#endif /* memory_h */
