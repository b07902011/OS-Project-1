#define _GNU_SOURCE
#include "process.h"
#include <sched.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <string.h>
void unit(){
	for(unsigned long i = 0; i < 1000000UL; i ++);
	return;
}



int execproc(proc *p){
	int pid = fork();
	if(pid == 0){
		unsigned long ssec, snsec, endsec, endnsec;
		char s[100] = "";
		p->pid = getpid();
		syscall(334, &ssec, &snsec);
		for(int i = 0; i < p->execTime; i ++){
			//fprintf(stderr, "%s in %d\n", p->name, i);
			unit();
		}
		syscall(334, &endsec, &endnsec);
		sprintf(s, "[Project1] %d %lu.%09lu %lu.%09lu\n\0", p->pid, ssec, snsec, endsec, endnsec);
		syscall(335, s, 100);
		exit(0);
	}
	return pid;
}

void assignCPU(int pid, int core){
	cpu_set_t mask;
	CPU_ZERO(&mask);
	CPU_SET(core, &mask);
	sched_setaffinity(pid, sizeof(mask), &mask);
	return;
}

void block(int pid){
	struct sched_param param;
	param.sched_priority = 0;
	int err = sched_setscheduler(pid, SCHED_IDLE, &param);
	//fprintf(stderr, "block %d, %d\n", pid, err);
}

void wakeup(int pid){
	struct sched_param param;
	param.sched_priority = 1;
	int err = sched_setscheduler(pid, SCHED_FIFO, &param);
	//fprintf(stderr, "wakeup %d , %d\n", pid, err);
}


