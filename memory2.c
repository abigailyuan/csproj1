#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h> // for open
#include <unistd.h> // for close
#include <assert.h>
#include "disk.h"
#include "memory2.h"

Segmentation* findHoleFirst(Segmentation* sHead, Segmentation* sTail, int size){
  Segmentation* hole = sHead;
  Segmentation current = *sHead;
  printf("current seg procss is %p\n", current->p);
  while(current != NULL){
    if(current->size >= size && current->p == NULL){
      
    }
  }
}
