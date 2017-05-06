#ifndef memory_h
#define memory_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h> // for open
#include <unistd.h> // for close
#include <assert.h>
#include "disk.h"
//#include "rrq.h"

typedef struct segmentation_t* Segmentation;

typedef struct rrq_t* rrq;

struct segmentation_t {
    Segmentation next;
    Process p;
    int size;
    Segmentation prev;
};

struct rrq_t {
    Segmentation s;
    rrq prev;
    rrq next;
};




//add a process to memory
extern void pushToMemory(Process p, Segmentation s);

//swap out a process from memory
extern Segmentation popFromMemory(Segmentation s);

//merge two Segmentation

extern Segmentation mergeSementation(Segmentation s1, Segmentation s2, Segmentation* sTail);

//find a hole
extern Segmentation findHole1(Segmentation* sHead, Segmentation* sTail, int size);

extern Segmentation findHole2();

extern Segmentation findHole3();




//push in rrq
extern void pushToRRQ(Segmentation* sPointer, rrq* rrqTailPointer, rrq* rrqHeadPointer);

//pop from rrq
extern Segmentation* popFromRRQ(rrq rrqHead);

//allocate CPU
extern void allocateCPU(rrq* rrqHeadPointer, rrq* rrqTailPointer, int quantumCounter);








#endif /* memory_h */
