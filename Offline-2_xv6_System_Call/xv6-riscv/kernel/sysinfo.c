#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "spinlock.h"
#include "proc.h"
#include "defs.h"

// Get current system info

int 
sysinfo(void) {
	int freemem = free_mem();
	int nproc = n_proc();

	printf("\n\nsysinfo system call prints:\n");
	printf("free-memory: %d bytes\n", freemem);
	printf("n_proc : %d\n\n\n", nproc);

	return 0;
}