/*
*	DKU Operating System Lab
*	    Lab1 (Scheduler Algorithm Simulator)
*	    Student id : 32170078
*	    Student name : 강정현
*
*   lab1_sched.c : 
*       - Lab1 source file.
*       - Must contains scueduler algorithm test code.
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

void input_data(Process* w);
void input_data2(Process* w);
void input_data3(Process* w);
void input_data4(Process* w);

/*
 * you need to implement scheduler simlator test code.
 *
 */

int main(int argc, char *argv[]) {
    Process workload[NUM];
    int table[NUM][MAX];

    printf("* Default WorkLoad *\n");
    input_data(workload);

    FIFO(workload, table);
    SJF(workload, table);
    RR(workload, table, 1);     // q = 1
    RR(workload, table, 4);     // q = 4
    MLFQ(workload, table, 1);   // q = 1
    MLFQ(workload, table,-2);   // q = 2^i
    Stride(workload, table);
    Lottery(workload, table);
}

void input_data(Process* w) {
    w[0].name = 'A';
    w[0].arrival_time = 0;
    w[0].service_time = 3;
    w[0].tickets = 100;

    w[1].name = 'B';
    w[1].arrival_time = 1;
    w[1].service_time = 7;
    w[1].tickets = 200;

    w[2].name = 'C';
    w[2].arrival_time = 2;
    w[2].service_time = 3;
    w[2].tickets = 300;

    w[3].name = 'D';
    w[3].arrival_time = 4;
    w[3].service_time = 4;
    w[3].tickets = 50;

    w[4].name = 'E';
    w[4].arrival_time = 5;
    w[4].service_time = 3;
    w[4].tickets = 150;
}