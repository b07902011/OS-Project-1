#include <sys/types.h>

void unit();
typedef struct process{
	char name[32];
	int readyTime, execTime, pid;
	unsigned long ssec, snsec, endsec, endnsec;
}proc;

void assignCPU(int pid, int core);
int execproc(proc *p);
void block(int pid);
void wakeup(int pid);
void print(proc *p);
