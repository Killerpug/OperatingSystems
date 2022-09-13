#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    int i;
    char *nargv[MAXARG];

    if(argc < 3 || (argv[1][0] < '0' || argv[1][0] > '9')){     //arguments must call at least 1 other syscall(traced syscall). Check first digit of mask to verify its int type.
        fprintf(2, "Usage: %s mask command\n", argv[0]);
        exit(1);
    }

    if (trace(atoi(argv[1])) < 0) {                // Call sys trace
        fprintf(2, "%s: trace failed\n", argv[0]);
        exit(1);
    }
    
    for(i = 2; i < argc && i < MAXARG; i++){    // Passes the program on which we will perform syscall-tracing, by making argv[2] the argument of new exec.
    	nargv[i-2] = argv[i];
    }
    exec(nargv[0], nargv);
    exit(0);
}