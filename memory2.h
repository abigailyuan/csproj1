#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h> // for open
#include <unistd.h> // for close
#include <assert.h>
#include "disk.h"

typedef struct segmentation_t* Segmentation;
struct segmentation_t {
    Segmentation next;
    Process p;
    int size;
    Segmentation prev;
};
//push to memory1
extern Process pushToMemory();

//pop from memory
extern Process popFromMemory();

//find hole
extern Segmentation* findHoleFirst(Segmentation* sHead, Segmentation* sTail, int size);

//mergeSementation
extern Segmentation mergeSementation();

//insert a new seg after given seg
extern Segmentation insert(Segmentation s, int size);

//push to rrq
extern void pushToRRQ(Process tail, int time_created, int process_id, int size, int job_time, int timein);

//popFromRRQ
extern Process popFromRRQ();

//number of process
extern int numprocess(Process head);

//finish process
extern void finishProcess();
