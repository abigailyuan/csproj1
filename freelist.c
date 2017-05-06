#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h> // for open
#include <unistd.h> // for close
#include <assert.h>
#include "disk.h"
#include "memory1.h"
#include "freelist.h"

void merge(freelist f);

void pushTofreelist(Segmentation* sPointer, freelist fHead){
  if(fHead != NULL){
    //search where to insert
    freelist newHole = malloc(sizeof(freelist));
    newHole->sPointer = sPointer;
  }else{

  }
}
Segmentation* popFromfreelist(Segmentation* sPointer, freelist fHead){

}

Segmentation* findPlace1(int size, freelist* fHeadPointer){

}

void merge(freelist f){

}
