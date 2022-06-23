// created by Killerpug
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
// sleep should pause for a user-specified number of ticks, each tick is approximately 100ms in xv6 implementation.


int main(int argc, char *argv[])
{
    if(argc <= 1){    
        fprintf(2, "Not enought argument: must specify the number of ticks to sleep");
        exit(0);
    }
    int ticks = atoi(argv[1]);
    sleep(ticks);

    exit(0);
}