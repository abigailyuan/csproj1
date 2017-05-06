#ifndef freelist_h
#define freelist_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h> // for open
#include <unistd.h> // for close
#include <assert.h>
#include "disk.h"
#include "memory1.h"



typedef struct freelist_t* freelist;
struct freelist_t {
    Segmentation* sPointer;
    freelist prev;
    freelist next;
};

//push in freelist
extern void pushTofreelist(Segmentation* sPointer, freelist fHead);

//pop from freelist
extern Segmentation* popFromfreelist(Segmentation* sPointer, freelist fHead);

//find a hole
extern Segmentation* findPlace1(int size, freelist* fHeadPointer);

//find a hole
extern Segmentation* findPlace2(int size, freelist* fHeadPointer);

//find a hole
extern Segmentation* findPlace3(int size, freelist* fHeadPointer);


#endif /* freelist_h */
