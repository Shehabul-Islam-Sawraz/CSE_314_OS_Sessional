#include "kernel/types.h"
#include "kernel/riscv.h"
#include "user/user.h"


void
sinfo() {
  if (sysinfo() < 0) {
    printf("FAIL: sysinfo failed");
    exit(1);
  }
}


void
testmem() {
  printf("\n\t\t\tTesting memory\n");
  printf("\nInitial State\n");
  sinfo();

  printf("Using up one more page (4094 bytes).\nFreemem should reduce by that much!\n");
  if((uint64)sbrk(PGSIZE) == 0xffffffffffffffff){
    printf("sbrk failed");
    exit(1);
  }

  sinfo();

  printf("Giving back that one more page to the pool (4094 bytes).\nFreemem should go back to the initial value!\n");
  
  if((uint64)sbrk(-PGSIZE) == 0xffffffffffffffff){
    printf("sbrk failed");
    exit(1);
  }

  sinfo();
}

void testproc() {
  int status;
  int pid;

  printf("\n\t\t\tTesting nproc\n");
  printf("\nInitial State\n");
  sinfo();

  pid = fork();
  if(pid < 0){
    printf("sysinfotest: fork failed\n");
    exit(1);
  }
  if(pid == 0){  // inside the child process
    printf("Created one new process. So nproc should increase by 1.");
    sinfo();
    exit(0);
  }

  wait(&status); // wait for the created child process to end
  printf("Created process ended. So nproc should go back to initial value.");
  sinfo(); 
}


int
main(int argc, char *argv[])
{
  printf("sysinfotest: start\n");
  testmem();
  testproc();
  printf("sysinfotest: done\n");
  exit(0);
}
