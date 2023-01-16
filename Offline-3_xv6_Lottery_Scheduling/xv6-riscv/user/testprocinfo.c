#include "kernel/pstat.h"
#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char* argv[])
{
    struct pstat ps;
    if (getpinfo(&ps) < 0)
    {
        printf("ps: getpinfo() failed\n");
        exit(1);
    }

    int i;
    printf("\nPID\tINUSE\tORIGINAL TICKETS\tCURRENT TICKETS\t\tTICKS\n");
    for (i=0; i < NPROC; i++)
    {
        if (ps.inuse[i] && ps.pid[i] != 0 &&  ps.tickets_current[i] >= 0)
        {
            printf("%d\t%d\t%d\t\t\t%d\t\t\t%d\n", ps.pid[i], ps.inuse[i], ps.tickets_original[i], ps.tickets_current[i], ps.time_slices[i]);
        }
    }

    exit(0);
}