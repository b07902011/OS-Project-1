#include "process.h"

#define FIFO 1
#define RR	2
#define SJF	3
#define PSJF 4
#define RRcycle 500
void scheduler(proc *procarr, int procnum, int policy);
