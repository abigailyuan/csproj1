//
//  memory.c
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
#include <assert.h>
#include <stdbool.h>
#include "disk.h"
#include "memory.h"


void insert(Process p, Segmentation current, FreeList current_hole);
void deleteFromFreeList(FreeList f, FreeList* fHead, FreeList* fTail);
void divideHole(FreeList current_hole, FreeList* fHead, FreeList* fTail, int size);


bool pushToMemory(Process p, int* time, int* memsize, int* usedSize, Segmentation s){

  if((*usedSize + p->size) < *memsize){
    (*usedSize) += p->size;
    s->p = p;
    s->size = p->size;

    printf("current used size is %d\n", *usedSize);
    return true;
  }
  return false;
}



// Segmentation findPlace(FreeList freeList, int size, FreeList* fHead, FreeList* fTail){
//   bool isEnough = false;
//   FreeList current_hole = *fHead;
//   Segmentation hole = *((*fHead)->s);
//   while(!isEnough){
//     if((hole)->size == size){
//       //delete from freeList
//       deleteFromFreeList(current_hole, fHead, fTail);
//       isEnough = true;
//       printf("the hole is found.\n");
//       printf("the hole size is %d\n", hole->size);
//       return hole;
//     }else if((hole)->size > size){
//       //if the size is larger than needed, divide into two seg
//       int sizeDiff = hole->size - size;
//       printf("the new size of the hole is %d\n", sizeDiff);
//       //create a new seg for the second half
//       FreeList f = malloc(sizeof(FreeList)*2 + sizeof(int)+ sizeof(Segmentation*));
//       assert(f);
//       f->s = NULL;
//       f->size = sizeDiff;
//       //if the hole is the head of freelist
//       if(current_hole->prev == NULL){
//         f->prev = NULL;
//         //if it is the only one
//         if(current_hole->next != NULL){
//           f->next = current_hole->next;
//         }
//       }else if(current_hole->next == NULL){
//         //if the hole is the tail of freelist
//         f->next = NULL;
//       }else{
//         //if the hole is in the middle
//         f->next = current_hole->next;
//         f->prev = current_hole->prev;
//       }
//       //change the flage to true when hole is found
//       isEnough = true;
//       printf("the hole is found.\n");
//       printf("the hole size is %d\n", hole->size);
//       return hole;
//       free(current_hole);
//     } else{
//       current_hole = current_hole->next;
//       //break when no hole is found
//       if(current_hole == NULL){
//         break;
//       }
//     }
//   }
//   //if no place return NULL
//   return NULL;
//
// }

Process nextOutFromRRQ(Process rrq){
  Process current_process = rrq;
  Process oldest = current_process;
  while(current_process != NULL){
    if(oldest->time_created > current_process->time_created){
      oldest = current_process;
      current_process = current_process->next;
    }else if(oldest->time_created == current_process->time_created){
      if(oldest->process_id > current_process->process_id){
        oldest = current_process;
      }
      current_process = current_process->next;
    }else{
      current_process = current_process->next;
    }
  }
  return oldest;
}

void deleteFromFreeList(FreeList f, FreeList* fHead, FreeList* fTail){
  //if f is the head
  if(f->prev == NULL){
    f->next->prev = NULL;
    *fHead = f->next;
    if(f->next == NULL){
      f->prev->next = NULL;
      *fTail = f->prev;
    }
  }else if(f->next == NULL){
    f->prev->next = NULL;
    *fTail = f->prev;
  }else{
    f->prev->next = f->next;
    f->next->prev = f->prev;
  }
  free(f);
}


int numOfPinRRQ(Process rrqHead){
  int num = 0;
  while(rrqHead != NULL){
    num++;
    rrqHead = rrqHead->next;
  }
  return num;
}

void removeFromRRQ(Process p, Process* rrqHead){
  //if it is the last one
  if(p->next == NULL){
    //if it is not the only one
    if(p->prev!=NULL){
      p->prev->next = NULL;
    }
    free(p);
  }else if(p->prev == NULL){
    *rrqHead = p->next;
    free(p);
  }else{
    p->prev->next = p->next;
    p->next->prev = p->prev;
    free(p);
  }
}

Segmentation findPlace(FreeList* fHead, FreeList* fTail, int size){
  FreeList current_hole = *fHead;
  Segmentation hole = NULL;
  while(current_hole != NULL){
    printf("current hole has size %d\n", current_hole->size);
    if(current_hole->size >= size){
      //change the hole
      hole =  *(current_hole->s);
      printf("the size of the hole to return is %d\n", current_hole->size);
      divideHole(current_hole, fHead, fTail, size);
      break;
    }else{
      current_hole = current_hole->next;
    }
  }
  if(hole == NULL){printf("no more holes.\n");}
  return hole;
}

// void divideHole(FreeList* fHead, FreeList* fTail, FreeList current_hole, int size){
//   int newSize = (current_hole->size) - size;
//   printf("new size is %d\n", newSize);
//   if(newSize == 0){
//     //delete the hole and change head and tail if needed
//     if(current_hole->prev == NULL && current_hole->next == NULL){
//       *fHead = NULL;
//       *fTail = NULL;
//       free(current_hole);
//     }else if(current_hole->prev == NULL){
//       current_hole->next->prev = NULL;
//       *fHead = current_hole->next;
//     }else if(current_hole->next == NULL){
//       current_hole->prev->next = NULL;
//       *fTail = current_hole->prev;
//       free(current_hole);
//     }else{
//       current_hole->prev->next = current_hole->next;
//       current_hole->next->prev = current_hole->prev;
//       free(current_hole);
//     }
//   }else{
//     //create new Segmentation
//     Segmentation newSeg = malloc(sizeof(Segmentation));
//     newSeg->size = newSize;
//     printf("newSeg size is %d\n", newSeg->size);
//     newSeg->prev = *(current_hole->s);
//     newSeg->next = (*(current_hole->s))->next;
//     newSeg->p = NULL;
//     //create new freeList
//     FreeList newF = malloc(sizeof(FreeList)*2 + sizeof(int)+ sizeof(Segmentation*));
//     assert(newF);
//     newF->s = &newSeg;
//     assert(newF->size);
//     newF->size = newSize;
//     printf("newF size is %d\n", newF->size);
//     newF->prev = current_hole->prev;
//     newF->next = current_hole->next;
//     *fHead = newF;
//     *fTail = newF;
//     free(current_hole);
//   }
//   printf("the new head size is %d\n", (*fHead)->size);
// }

Segmentation createNewSeg(int size){

  Segmentation s = malloc(sizeof(Segmentation)*2+ sizeof(int)+ sizeof(Process));
  assert(s);
  s->size = size;
  s->p = NULL;
  s->prev = NULL;
  s->next = NULL;
  //printf("aaaaaaaaa\n");
  return s;

//  return NULL;
}

void divideHole(FreeList current_hole, FreeList* fHead, FreeList* fTail, int size){
  int newSize = current_hole->size - size;
  //add a new seg in memory
  Segmentation original = *(current_hole->s);
  Segmentation newSeg = createNewSeg(newSize);
  printf("aaaaaa\n");

  newSeg->prev = original;

  newSeg->next = original->next;
//printf("bbb\n");
  printf("original->next->prev %p", original->next);
  if(original->next != NULL && original->next->prev != NULL){
    printf("inside if\n");
    original->next->prev = newSeg;
    printf("dd");
  }
  //printf("ccb\n");
  original->next = newSeg;

  //add new free list into free list and remove the old one
  current_hole->s = &newSeg;
  current_hole->size = newSeg->size;


}

Process swapOut(Segmentation head, Segmentation s){
  Process pOut = s->p;
  s->p = NULL;
  //add to free list
  return pOut;
}
