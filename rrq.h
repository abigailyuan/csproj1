#ifndef rrq_h
#define rrq_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h> // for open
#include <unistd.h> // for close
#include <assert.h>
#include "disk.h"
#include "memory1.h"

typedef struct rrq_t* rrq;
struct rrq_t {
    Segmentation* sPointer;
    rrq prev;
    rrq next;
};

//push in rrq
extern void pushToRRQ(Segmentation* sPointer, rrq* rrqTailPointer);

//pop from rrq
extern Segmentation* popFromRRQ(rrq rrqHead);

//allocate CPU
extern void allocateCPU(rrq* rrqHeadPointer, rrq* rrqTailPointer);



#endif /* rrq_h */
