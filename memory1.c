#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h> // for open
#include <unistd.h> // for close
#include <assert.h>
#include <stdbool.h>
#include "disk.h"
#include "memory1.h"
//#include "rrq.h"


Segmentation divide(Segmentation s, int size);

void pushToMemory(Process p, Segmentation s){
  s->p = p;
  s->size = p->size;
}

Segmentation popFromMemory(Segmentation s){
  s->p = NULL;
  return s;
}

Segmentation mergeSementation(Segmentation s1, Segmentation s2, Segmentation* sTail){
  s1->size += s2->size;
  s1->next = s2->next;
  if(s2->next != NULL){
    s2->next->prev = s1;
  }else{
    s1->next = NULL;
    sTail = &s1;
  }
  return s1;
}

Segmentation findHole1(Segmentation* sHead, Segmentation* sTail, int size){
  Segmentation current = *sHead;
  while(current != NULL){
    if(current->size>=size){
      if(current->size > size){
        divide(current, size);
      }
      return current;
    }else{
      current = current->next;
    }
  }
  return NULL;
}
Segmentation divide(Segmentation s, int size){
  Segmentation newS = malloc(sizeof(Segmentation));
  int newSize = s->size - size;
  newS->p = NULL;
  s->size = size;
  //if s is not the last one
  if(s->next != NULL){
    newS->next = s->next;
    newS->next->prev = newS;
  }else{
    newS->next = NULL;
  }
  newS->prev = s;
  s->next = newS;
  newS->size = newSize;
  printf("the new hole size is %d\n", newS->size);
  return newS;
}

void pushToRRQ(Segmentation s, rrq rrqTail, rrq rrqHead){
  rrq newTail = malloc(sizeof(rrq));
  newTail->s = s;
  newTail->next = NULL;
  newTail->prev = rrqTail;
  rrqTail = newTail;
  if(rrqHead == NULL){
    rrqHead = newTail;
  }
  // assert(newTail);
  // printf("sPointer at %p\n", *sPointer);
  // newTail->sPointer = sPointer;
  // newTail->prev = *rrqTailPointer;
  // (*rrqTailPointer)->next = newTail;
  // *rrqTailPointer = newTail;
  // if(rrqHeadPointer == NULL){
  //   *rrqHeadPointer = newTail;
  //   //(*rrqHeadPointer)->sPointer = newTail->sPointer;
  // }
  // // printf("when push to rrq, s at %p\n", newTail->sPointer);
  // // printf("newTail prev at %p\n", newTail->prev);
  // // printf("newTail next at %p\n", newTail->next);
  // // printf("rrqHead s at %p\n", (*rrqHeadPointer)->sPointer);
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
   rrq current = *rrqHeadPointer;
   if(current->next != NULL){
     current->next->prev = NULL;
     rrqHeadPointer = &(current->next);
     (*rrqTailPointer)->next = current;
     current->prev = *rrqTailPointer;
     rrqTailPointer = &current;
   }
 }
