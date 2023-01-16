#include "kernel/types.h"
#include "kernel/pstat.h"
#include "user/user.h"


int main(int argc, char * argv[])
{
    //printf("test_Ticket\n");
    
    // int number = atoi(argv[1]);
    
    // if (settickets(number) < 0)
    // {
    //     printf("settickets() failed\n");
    //     exit(1);
    // }

    // while (1)
    // {
    //     /* code */
    // }
    
    // exit(0); //eq to return zero

    /**
     * 
     * fork test
     * 
     **/


    // printf(1, "testFork\n");
    
    int number = atoi(argv[1]);
    settickets(number);

    int val = fork();

    if(val == 0) printf("\nFork successful\n");
    else if (val < 0) printf("\nFork unsuccessful\n");

    while (1)
    {
        /* code */
    }
    
    exit(0);//eq to return zero

}