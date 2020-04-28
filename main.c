#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sched.h>
#include <errno.h>
#include <unistd.h>
#include "scheduler.h"

int main(){
	int procn, p;
	char policy[20];
	scanf("%s%d", policy, &procn);
	proc *procarr = (proc *)malloc(procn * sizeof(proc));
	for(int i = 0; i < procn; i ++){
		scanf("%s%d%d", procarr[i].name, &procarr[i].readyTime, &procarr[i].execTime);
	}
	if(!strcmp(policy, "FIFO"))
		p = FIFO;
	else if(!strcmp(policy, "RR"))
		p = RR;
	else if(!strcmp(policy, "SJF"))
		p = SJF;
	else
		p = PSJF;
	scheduler(procarr, procn, p);
	return 0;
}
