#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h> // for open
#include <unistd.h> // for close
#include <assert.h>
#include <stdbool.h>
#include "disk.h"
#include "memory1.h"
#include "rrq.h"

void pushToRRQ(Segmentation* sPointer, rrq* rrqTailPointer){
  rrq newTail = malloc(sizeof(rrq));
  assert(newTail);
  newTail->sPointer = sPointer;
  newTail->prev = *rrqTailPointer;
  (*rrqTailPointer)->next = newTail;
  rrqTailPointer = &newTail;
}

 Segmentation* popFromRRQ(rrq rrqHead){
   rrq* oldest = &rrqHead;
   rrq current = rrqHead;
   while(current != NULL){
     int current_time = (*(current->sPointer))->p->time_created;
     int oldest_time =  (*((*oldest)->sPointer))->p->time_created;
     if(current_time<oldest_time){
       oldest = &current;
     }else if(current_time == oldest_time){
       int current_id = (*(current->sPointer))->p->process_id;
       int oldest_id = (*((*oldest)->sPointer))->p->process_id;
       if(current_id<oldest_id){
         oldest= &current;
       }
     }
     current = current->next;
   }
   //create the seg
   Segmentation* olds = (*oldest)->sPointer;
   if((*oldest)->prev == NULL && (*oldest)->next == NULL){
     rrqHead = NULL;
   }else if((*oldest)->prev == NULL){
     (*oldest)->next->prev = NULL;
   }else if((*oldest)->next == NULL){
     (*oldest)->prev->next = NULL;
   }else{
     (*oldest)->prev->next = (*oldest)->next;
     (*oldest)->next->prev = (*oldest)->prev;
   }
   free(oldest);
   return olds;
 }

 void allocateCPU(rrq* rrqHeadPointer, rrq* rrqTailPointer, int quantumCounter){

 }
