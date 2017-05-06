#include        <stdio.h>
#include        <stdlib.h>
#include        <string.h>
#include <fcntl.h> // for open
#include <unistd.h> // for close
#include <assert.h>
#include <stdbool.h>
#include <math.h>
#include "disk.h"
//#include "memory3.h"
//#include "freelist.h"
//#include "rrq.h"




/* Extern declarations: */

extern  int     optind;
extern  char    *optarg;

void read_file(char* file_name, List *list);
//void swap(Process* head, FreeList* fHead, FreeList* fTail, int time, int memsize, int usedSize);
//void schedule(int* quantumCounter, rrq rrqHead, rrq rrqTail, int* numProcesses, int quantum);
//void swap(Process Phead, Segmentation sHead, Segmentation sTail, int time, rrq rrqHead, int* numProcesses, Process Ptail, rrq rrqTail, rrq rrqHead);



int main(int argc, char** argv)
{
    char input;
    int quantum = 7;        //default length of quantum
    int memsize = 1000;    //default main memory size
    int usedSize = 0;   //default used size in memory
    char* algorithm_name = "first";   //default algorithm
    char* file_name = "input.txt";    //default file name
    // Segmentation memory = malloc(sizeof(Segmentation));//default memory head
    // Segmentation* mHead = malloc(sizeof(Segmentation));
    // Segmentation* mTail = malloc(sizeof(Segmentation));
    // FreeList freeList = malloc(sizeof(FreeList)*2 + sizeof(int)+ sizeof(Segmentation*)); //a hole list in memory
    // FreeList* fHead = malloc(sizeof(FreeList));
    // FreeList* fTail = malloc(sizeof(FreeList));
    // Process rrq = malloc(sizeof(Process));  //round robin queue
    // Process* rrqHead = rrq;
    // Process* rrqTail = rrq;
    // bool* E1 = malloc(sizeof(bool));
    // bool* E2 = malloc(sizeof(bool));lsls
    // bool* E3 = malloc(sizeof(bool));
    // *E1 = false;
    // *E2 = false;
    // *E3  =false;


    //get input
    while ((input = getopt(argc, argv, "a:f:m:q:")) != EOF)
    {
        switch ( input )
        {

            case 'f':
                //read file
                //strcpy(optarg,file_name);
                file_name = optarg;
                break;

            case 'a':
            	//choose an algorithm
                if(strcmp(optarg, "best") == 0)
                    algorithm_name = optarg;
                else if(strcmp(optarg, "first") == 0)
                    algorithm_name = optarg;
                else if(strcmp(optarg, "worst") == 0)
                    algorithm_name = optarg;
                break;

            case 'm':
            	//choose mainmemory size
                memsize = atoi(optarg);
                break;

            case 'q':
            	//choose length of quantum
            	quantum = atoi(optarg);
            	break;

            default:
                // do something
                break;
        }
    }
    // a debugging step is included to display the value of args read
    printf("main memory size = %d algorithm = %s\n", memsize, algorithm_name);
    printf("quantun = %d process file = %s\n", quantum, file_name);
    printf("--------------------------------------------------------\n");




    //read processes into disk
    List all_p = NULL;
    read_file(file_name, &all_p);
    printf("number of process is %d\n", numOfProcess(all_p->head));


    bool isFinished = false;
    int time = 0;
    int numProcesses = numOfProcess(all_p->head);
    int quantumCounter = 0;

    List disk = NULL;
    List memory = NULL;
    List rrq = NULL;
    int numHoles = 1;
    int numProcess = 0;
    int usage = 0;

    bool processFinish = false;
    Process current_running = NULL;

    while(!isFinished){
    	    int event = 0;

    	    // process generator
    	    Node temp1 = all_p->head;
    	    while (temp1 != NULL) {
    	    	    if (temp1->data->time_created == time) {
    	    	    	    push(&disk, temp1->data);
    	    	    	    pop(&all_p);
    	    	    }
    	    	    temp1 = temp1->next;
    	    }

    	    // observe event
    	    if (disk != NULL && memory == NULL) {
    	    	    event = 1;
    	    } else if (current_running != NULL && current_running->job_time == 0) {
    	    	    event = 2;
    	    } else if (quantumCounter == quantum) {

    	    	    quantumCounter = 0;
    	    	    event = 3;
    	    }

    	    // do something for some event
    	    if (event == 1) {
    	    	    printf("enter event 1\n");
    	    	    Process hole = (Process)malloc(sizeof(struct process_t));
    	    	    hole->process_id = -1;
    	    	    hole->timein = -1;
    	    	    hole->size = memsize;
    	    	    push(&memory, hole);
    	    	    numHoles++;


    	    	    // swap
    	    	    Process p = pop(&disk);// search longest p
    	    	    //Process p = popLongest(&disk, time);//da gou
    	    	    p->timein = time;
    	    	   //push(&memory, p);// search for hole and insert before that hole
    	    	     Node node = findHole(&memory, p);
	              while(node == NULL){
              			Process tobeRemoved = popOldest(&rrq);
              			popFromMemory(&memory, tobeRemoved);
                    usage -= tobeRemoved->size;
              			tobeRemoved->timein = -1;
              			tobeRemoved->time_created = time;
              			numHoles += 1; //todo check merge
              			push(&disk, tobeRemoved);
              			node = findHole(&memory, p);
		             }
                 printf("push %d to rrq\n", p->process_id);
    	    	    push(&rrq, p);//dagou
    	    	    //hole found
			    insertBefore(node, p, &memory);
			    numProcess++;
          usage += p->size;
			    p->timein = time;

			    if(node->data->size > p->size){
			    	    node->data->size -= p->size;
			    }else{
			    	    numHoles -= 1;
			    }

    	    	    // schedule
    	    	    if (rrq->head != NULL) {
    	    	    	    current_running = rrq->head->data;

    	    	    }

    	    	     Node temp = rrq->head;

                int use = (int)ceil(100.0*usage/memsize);
        printf("time %d, %d loaded, numprocesses=%d, numHoles=%d, memusage=%d%%\n", time, p->process_id, numProcess, numHoles, use);
	   /* while (temp != NULL) {
		    fprintf(stdout, "time: %d, id: %d, size: %d\n", time, temp->data->process_id, temp->data->size);
		    temp = temp->next;
	    }
	    fprintf(stdout, "------------------------\n");*/
    	    	    /*Node temp = memory->head;
		    while (temp != NULL) {
			    fprintf(stdout, "event1: id: %d, size: %d\n", temp->data->process_id, temp->data->size);
			    temp = temp->next;
		    }
		    fprintf(stdout, "------------------------\n");*/

    	    } else if (event == 2) {
    	    	    printf("enter event 2\n");

    	    	    // swap
    	    	    if (disk->head != NULL) {
			    Process p = pop(&disk);// search longest p
			    //Process p = popLongest(&disk, time);//da gou
			    p->timein = time;
			    //push(&memory, p);// search for hole and insert before that hole
			    Node node = findHole(&memory, p);
			    while(node == NULL){
			    	    Process tobeRemoved = popOldest(&rrq);
			    	    Node n = popFromMemory(&memory, tobeRemoved);
			    	    numProcess -= 1;
                usage -= tobeRemoved->size;
			    	    tobeRemoved->timein = -1;
			    	    tobeRemoved->time_created = time;
                int changehole = 0;
                if(n->prev != NULL){
                  if(n->prev->data->timein == -1){
                    //merge with prev
                    n->prev->data->size += n->data->size;
                    //if n is not tail
                    if(n->next != NULL){
                      n->prev->next = n->next;
                    }else{
                      //if nn is tail
                      n->prev->next = NULL;
                      memory->tail = n->prev;
                    }
                    changehole = -1;//merge once
                    n = n->prev;
                  }
                }
                if(n->next != NULL){
                  if(n->next->data->timein == -1){
                    n->data->size += n->next->data->size;
                    if(n->next->next != NULL){
                      //n next is not tail
                      n->next->next->prev = n;
                      n->next = n->next->next;
                    }else{
                      //n next is tail
                      n->next = NULL;
                      memory->tail = n;
                    }
                    if(changehole == -1){
                      //merged twice
                      changehole = -2;
                    }
                  }
                }
                if(changehole == -2){
                  numHoles -= 1;
                }else if(changehole == 0){
                  numHoles += 1;
                }
			    	    //numHoles += 1; //todo check merge
                printf("push to disk %d\n", tobeRemoved->process_id);
			    	    push(&disk, tobeRemoved);
			    	    node = findHole(&memory, p);
			    }
          //printf("push %d to rrq\n", p->process_id);
			    push(&rrq, p); // dagou
			    //hole found
			   insertBefore(node, p, &memory);
			    numProcess++;
          usage += p->size;
			    p->timein = time;
          int use = (int)ceil(100.0*usage/memsize);
          printf("time %d, %d loaded, numprocesses=%d, numHoles=%d, memusage=%d%%\n", time, p->process_id, numProcess, numHoles, use);

			    if(node->data->size > p->size){
			    	    node->data->size -= p->size;
			    }else{
			    	    numHoles -= 1;
			    }
    	    	    }

    	    	    // schedule
    	    	    if (current_running == rrq->head->data) {
			    Process popped = pop(&rrq);
    	    	    }

    	    	    if (rrq->head != NULL) {
    	    	    	    current_running = rrq->head->data;

    	    	    }
    	    	      Node temp = rrq->head;

	    /*while (temp != NULL) {
		    fprintf(stdout, "time: %d, id: %d, size: %d\n", time, temp->data->process_id, temp->data->size);
		    temp = temp->next;
	    }
	    fprintf(stdout, "------------------------\n");*/
    	    	    /*Node temp = memory->head;
		    while (temp != NULL) {
			    fprintf(stdout, "event2: id: %d, size: %d\n", temp->data->process_id, temp->data->size);
			    temp = temp->next;
		    }
		    fprintf(stdout, "------------------------\n");*/


    	    } else if (event == 3) {
    	    	    //printf("enter event 3\n");

    	    	    quantumCounter = 0;


                // swap
                Process p = pop(&disk);// search longest p
                //Process p = popLongest(&disk, time);//da gou
                p->timein = time;
               //push(&memory, p);// search for hole and insert before that hole
               Node node = findHole(&memory, p);
              while(node == NULL){
                Process tobeRemoved = popOldest(&rrq);
                Node n = popFromMemory(&memory, tobeRemoved);
                numProcess--;
                usage -= tobeRemoved->size;

                tobeRemoved->timein = -1;
                tobeRemoved->time_created = time;
                int changehole = 0;
                if(n->prev != NULL){
                  if(n->prev->data->timein == -1){
                    //merge with prev
                    n->prev->data->size += n->data->size;
                    //if n is not tail
                    if(n->next != NULL){
                      n->prev->next = n->next;
                    }else{
                      //if nn is tail
                      n->prev->next = NULL;
                      memory->tail = n->prev;
                    }
                    changehole = -1;//merge once
                    n = n->prev;
                  }
                }
                if(n->next != NULL){
                  if(n->next->data->timein == -1){
                    n->data->size += n->next->data->size;
                    if(n->next->next != NULL){
                      //n next is not tail
                      n->next->next->prev = n;
                      n->next = n->next->next;
                    }else{
                      //n next is tail
                      n->next = NULL;
                      memory->tail = n;
                    }
                    if(changehole == -1){
                      //merged twice
                      changehole = -2;
                    }
                  }
                }
                if(changehole == -2){
                  numHoles -= 1;
                }else if(changehole == 0){
                  numHoles += 1;
                }
                //numHoles += 1; //todo check merge
                //printf("push %d to diskkkkkkkkk\n", tobeRemoved->process_id);
                push(&disk, tobeRemoved);
                node = findHole(&memory, p);
              }
              //printf("push %d to rrq\n", p->process_id);
                push(&rrq, p);//dagou
                //hole found

          insertBefore(node, p, &memory);

          numProcess++;
          usage += p->size;
          p->timein = time;

          if(node->data->size > p->size){
                node->data->size -= p->size;
          }else{
                numHoles -= 1;
          }

    	    	    // schedule
    	    	    if (current_running == rrq->head->data) {
			    Process popped = pop(&rrq);
			    push(&rrq, popped);
    	    	    }

    	    	    if (rrq->head != NULL) {
    	    	    	    current_running = rrq->head->data;

    	    	    }
                int use = (int)ceil(100.0*usage/memsize);
        printf("time %d, %d loaded, numprocesses=%d, numHoles=%d, memusage=%d%%\n", time, p->process_id, numProcess, numHoles, use);
    	/*Node temp = rrq->head;//debug
	    while (temp != NULL) {
		    fprintf(stdout, "time: %d, id: %d, size: %d\n", time, temp->data->process_id, temp->data->size);
		    temp = temp->next;
	    }
	    fprintf(stdout, "------------------------\n");*/

    	    	    /*Node temp = memory->head;
		    while (temp != NULL) {
			    fprintf(stdout, "event3: id: %d, size: %d\n", temp->data->process_id, temp->data->size);
			    temp = temp->next;
		    }
		    fprintf(stdout, "------------------------\n");*/
      }


    	   /* Node temp = memory->head;
	    while (temp != NULL) {
		    fprintf(stdout, "time: %d, id: %d, size: %d\n", time, temp->data->process_id, temp->data->size);
		    temp = temp->next;
	    }
	    fprintf(stdout, "------------------------\n");
    	    */


    	    // step
    	    quantumCounter += 1;
    	    //printf("quantumCounter is %d\n", quantumCounter);

    	    /*if (current_running != NULL) {
    	    	    current_running->job_time -= 1;
    	    	    printf("current process is %d, job_time is %d\n", current_running->process_id, current_running->job_time);
    	    }*/

    	    // debug
    	    if (all_p->head == NULL) {
    	    	    break;
    	    }
    	    time++;

    }


    /*Node temp = disk->head;
    while (temp != NULL) {
    	    fprintf(stdout, "one: %d, two: %d, three: %d, four: %d\n", temp->data->time_created,
    	    	    temp->data->process_id, temp->data->size, temp->data->job_time);
    	    temp = temp->next;
    }
    printf("time: %d\n", time);*/

    return 0;
}

void read_file(char* file_name, List *list){
  FILE *f = fopen(file_name, "r");
  int one, two, three, four;
  while(fscanf(f, "%d %d %d %d\n", &one, &two, &three, &four) == 4){
	Process p = malloc(sizeof(struct process_t));
	p->time_created = one;
	p->process_id = two;
	p->size = three;
	p->job_time = four;
	p->timein = -1;
	push(list, p);
  }
  fclose(f);
  printf("file has been read.\n");
}
