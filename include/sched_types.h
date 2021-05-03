/*
*	DKU Operating System Lab
*	    Lab1 (Scheduler Algorithm Simulator)
*	    Student id : 32170078
*	    Student name : 강정현
*
*   lab1_sched_types.h : 
*       - lab1 header file.
*       - must contains scueduler algorithm function's declations.
*
*/

#ifndef _LAB1_HEADER_H
#define _LAB1_HEADER_H


/*
 * You need to Declare functions in  here
 */

#define MAX 20  // Max time
#define NUM 5   // Process number
#define LAYER 10    // Layer of priority queue
#define INFI 100000

typedef struct {
    char name;
    int arrival_time;
    int service_time;
    int tickets;
} Process;

typedef struct {
    int *data;
    int rear;
    int front;
} Queue;

void FIFO(Process w[], int t[NUM][MAX]);
void SJF(Process w[], int t[NUM][MAX]);
void RR(Process w[], int t[NUM][MAX], int quantum);
void MLFQ(Process w[], int i[NUM][MAX], int quantum);
void Lottery(Process w[], int t[NUM][MAX]);
void Stride(Process *p, int t[NUM][MAX]);

#endif /* LAB1_HEADER_H*/
