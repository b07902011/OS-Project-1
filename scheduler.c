#define _GNU_SOURCE
#include "scheduler.h"
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sched.h>
#include <sys/syscall.h>
#include <sys/types.h>

int cmpbyready(const void *a, const void *b) {
	return ((proc *)a)->readyTime - ((proc *)b)->readyTime;
}

typedef struct Q{
	int p;
	struct Q *next;
	}q;

q *head = NULL, *tail = NULL;
int isEmpty(){
	if(head == NULL)
		return 1;
	return 0;
}

q * create(int n){
	q *new = (q*)malloc(sizeof(q));
	new->p = n;
	new->next = NULL;
	return new;
}

void enq(int n){
	if(head == NULL){
		head = tail = create(n);
	}
	else{
		tail->next = create(n);
		tail = tail->next;
	}
	return;
}

int deq(){
	q *temp = head->next;
	int p = head->p;
	free(head);
	head = temp;
	return p;
}

void scheduler(proc *procarr, int procnum, int policy){
	assignCPU(getpid(), 0);
	wakeup(getpid());
	qsort(procarr, procnum, sizeof(proc), cmpbyready);
	int runningproc = -1, time = 0, waitingprocnum = 0, lastswitchtime = -1;
	while(1){
		for(;waitingprocnum < procnum && procarr[waitingprocnum].readyTime == time; waitingprocnum ++){
			procarr[waitingprocnum].pid = execproc(procarr + waitingprocnum);
			assignCPU(procarr[waitingprocnum].pid, 1);
			block(procarr[waitingprocnum].pid);
			printf("%s %d\n", procarr[waitingprocnum].name, procarr[waitingprocnum].pid);
			fflush(stdout);
			enq(waitingprocnum);
		}
		if(runningproc == -1 || procarr[runningproc].execTime <= 0){
			if(runningproc != -1){
				waitpid(procarr[runningproc].pid, NULL, 0);
			}
			runningproc = -1;
			if(policy == FIFO || policy == RR){
				if(!isEmpty()){
					runningproc = deq();
					lastswitchtime = time;
				}
				else
					runningproc = -1;
			}
			else{
				int min = 2147483647;
				for(int i = 0; i < waitingprocnum; i ++){
					if(procarr[i].execTime > 0 && procarr[i].execTime < min){
						min = procarr[i].execTime;
						runningproc = i;
					}
				}
			}
			if(runningproc == -1 && waitingprocnum == procnum){
				break;
			}
			else if(runningproc != -1){
				wakeup(procarr[runningproc].pid);
			}
		}
		else if(policy == PSJF){
			int newrunning = runningproc;
			for(int i = 0 ; i < waitingprocnum; i ++){
				if(procarr[i].execTime > 0 && procarr[newrunning].execTime > procarr[i].execTime)
					newrunning = i;
			}
			if(newrunning != runningproc){
				block(procarr[runningproc].pid);
				runningproc = newrunning;
				wakeup(procarr[runningproc].pid);
			}
		}
		else if(policy == RR){
			 if(time - lastswitchtime == RRcycle){
				 block(procarr[runningproc].pid);
				 enq(runningproc);
				 if(!isEmpty()){
					 runningproc = deq();
				 }
				 else
					 runningproc = -1;
				 lastswitchtime = time;
				 if(runningproc != -1)
					wakeup(procarr[runningproc].pid);
			 }
		}
		if(runningproc != -1)
			procarr[runningproc].execTime --;
		//fprintf(stderr, "main in %d\n", time);
		unit();
		time ++;

	}
	return;
}
