/*
*	DKU Operating System Lab
*	    Lab1 (Scheduler Algorithm Simulator)
*	    Student id : 32170078
*	    Student name : 강정현
*
*   lab1_sched.c :
*       - Lab1 source file.
*       - Must contains scueduler algorithm function'definition.
*
*/

#include <aio.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <assert.h>
#include <pthread.h>
#include <asm/unistd.h>

#include "lab1_sched_types.h"

/*
 * you need to implement FCFS, RR, SPN, SRT, HRRN, MLFQ scheduler. 
 */

/* Main methods */
void print_result(Process w[], int p[][MAX]) {
    printf("   0");
    for (int i=NUM; i<=MAX; i+=NUM)
        printf("%20d", i);
    printf("\n   ");

    for (int i=0; i<MAX; i++)
        printf("|---");
    printf("|\n");

    for (int i=0; i<NUM; i++) {
        printf("%c  ", w[i].name);
        for (int j=0; j<MAX; j++)
            if (p[i][j])
                //printf(" OOO");
                printf("%3c ", i+'A');
            else
                printf("    ");
        printf("\n");
    }
    printf("\n");
}

void init_table(int t[NUM][MAX]) {
    for (int i=0; i<NUM; i++)
        for (int j=0; j<MAX; j++)
            t[i][j]=0;
}

/* Queue methods */
void qinit(Queue *q) {
    q->front = q->rear = 0; 
    q->data = (int*)malloc(sizeof(int)*(NUM+1));
}

void enqueue(Queue *q, int data){
    if (((q->rear + 1) % NUM == q->front))
        return;
    else {
        q->rear = (q->rear + 1) % (NUM+1);    
        q->data[q->rear] = data;
    }
}

int dequeue(Queue *q) {
    q->front = (q->front + 1) % (NUM+1);
    return q->data[q->front];
}

int qempty(Queue *q) {
    if (q->front == q->rear) return 1;
    else return 0;
}

int allempty(Queue queue[LAYER]) {
	for (int i=0; i<LAYER; i++)
		if (!qempty(&queue[i]))
			return 0;
	return 1;
}

/* FIFO */
void FIFO(Process w[], int t[NUM][MAX]) {
    init_table(t);
    int next = -1;
    int temp = 0;
    Queue queue;
    qinit(&queue);
    for (int run_time=0; run_time<MAX; run_time++) {
        for (int i=0; i<NUM; i++) {
            if (w[i].arrival_time == run_time)
                enqueue(&queue, i);
        }
        if (temp == run_time && !qempty(&queue)) {
            next = dequeue(&queue);
            for (int i=0; i < w[next].service_time; i++)
                t[next][temp++] = 1;
        }
    }
    printf("\t FIFO\n");
    print_result(w, t);
}

/* SJF */
void SJF(Process w[], int t[NUM][MAX]) {
    init_table(t);
    int min, next;
    int finish[NUM] = {0};
    for (int run_time=0; run_time<MAX; run_time) {
        min = INFI;
        for (int i=0; i<NUM; i++) {
            if (!finish[i] && w[i].arrival_time <= run_time)
                if (w[i].service_time < min) {
                    min = w[i].service_time;
                    next = i;
            }
        }
        for (int j=0; j < min; j++)
            t[next][run_time+j] = 1;
        finish[next] = 1;
        run_time += min;
    }
    printf("\t SJF\n");
    print_result(w, t);
}

/* RR */
void RR(Process w[], int t[NUM][MAX], int quantum) {
    init_table(t);
    Queue queue;
    qinit(&queue);
    int remain[NUM];
    int running = -1;
    int next = -1;

    for (int i=0; i<NUM; i++)
    	remain[i] = w[i].service_time;
    for (int run_time=0; run_time<MAX; run_time++) {
        for (int i=0; i<NUM; i++)
            if (w[i].arrival_time == run_time)
                enqueue(&queue, i);
        if (running<0) {
            if(next>=0)
                enqueue(&queue,next);
            if(!qempty(&queue)) {
                next = dequeue(&queue);
                running = 0;
            }
        }
        if (running>=0) {
            t[next][run_time] = 1;
            remain[next]--;
            running++;
            if(running==quantum)
		        running = -1;
	        if(remain[next] == 0) {
                running = -1;
		        next = -1;
            }
        }
    }
    printf("\t RR (q = %d)\n", quantum);
    print_result(w, t);
}

/* MLFQ */
void MLFQ(Process w[], int t[NUM][MAX], int quant) {
	init_table(t);
    Queue queue[LAYER];
	int next = -1;
	int running = -1;
    int remain[NUM], quantum[LAYER];
	int priority[NUM] = {0};
	
	for (int i=0; i<NUM; i++)
		remain[i] = w[i].service_time;
	for (int i=0; i<LAYER; i++)
		qinit(&queue[i]);
	if (quant<0) {
		int temp = -quant;
		quantum[0] = 1;
		for (int i=1; i<9; i++)
			quantum[i] = quantum[i-1]*temp;
	}
	else
		for (int i=0; i<LAYER; i++)
			quantum[i] = quant;
	
	for (int run_time=0; run_time<MAX; run_time++) {
		for (int i=0; i<NUM; i++)
			if (w[i].arrival_time == run_time)
				enqueue(&queue[0], i);
		
		if (running < 0) {
			if (next>=0) {
				if (allempty(queue) || priority[next] >= LAYER)
						priority[next]--;
				enqueue(&queue[priority[next]], next);
				}
			for (int i=0; i<LAYER; i++) {
				if (!qempty(&queue[i])) {
						next = dequeue(&queue[i]);
						running = 0;
						break;
				}
			}
		}
		if (running >= 0) {
			t[next][run_time] = 1;
			remain[next]--;
			running++;
			if (running==quantum[priority[next]]) {
				running = -1;
				priority[next]++;
			}
			if (remain[next] == 0) {
				running = -1;
				next = -1;
			}
		}
	}
    if (quant<0)
        printf("\tMLFQ (q = %d^i)\n", -quant);
    else
        printf("\t MLFQ (q = %d)\n", quant);
    print_result(w, t);
}

/* Lottery */
void Lottery(Process w[], int t[NUM][MAX]) {
    init_table(t);
    srand(time(NULL));
    int counter = 0;
    int totaltickets;
    int next, winner;
    int remain[NUM];
    int job_is_in[NUM] = {0};
	for (int i=0; i<NUM; i++)
		remain[i] = w[i].service_time;

    for (int run_time=0; run_time<MAX; run_time++) {
        totaltickets=0;
        for (int i=0; i<NUM; i++) {
            if (w[i].arrival_time == run_time)
                job_is_in[i] = 1;
            if (job_is_in[i])
                totaltickets += w[i].tickets;
        }
        winner = rand() % (totaltickets+1);
        for (int i=0; i<NUM; i++) {
            if (job_is_in[i]) {
                if (winner < w[i].tickets) {
                    next = i;
                    break;
                }
                else
                    winner -= w[i].tickets;
            }
        }
        t[next][run_time] = 1;
        remain[next]--;
        if (remain[next]<=0)
            job_is_in[next] = 0;
    }
    printf("\t Lottery\n");
    print_result(w, t);
}

void Stride(Process w[], int t[NUM][MAX]) {
    init_table(t);
    int stride[NUM];
    int remain[NUM];
    int pass_val[NUM];
    for (int i=0; i<NUM; i++) {
        pass_val[i] = 0;
        stride[i] = 1000 / w[i].tickets;
        remain[i] = w[i].service_time;
    }

    for(int i = 0; i < MAX; i++) {
        int min_idx = 0;
        int min_pass = 1e9; //가장 작은 pass val 을 구하기위한 임의의 숫자

        /* 가장 작은 pass val을 가진 process의 i ndex 구하기*/
        for (int j = 0; j < NUM; j++) {
            if(pass_val[j] < min_pass && remain[j]>0) {
                min_pass = pass_val[j];
                min_idx = j;
            }
        }
        if (min_pass == 1e9) break;
        t[min_idx][i] = 1;
        remain[min_idx]--;
        pass_val[min_idx] += stride[min_idx];   
    }

    printf("\t Stride\n");
    print_result(w, t);
}