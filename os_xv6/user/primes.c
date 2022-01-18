// created by Killerpug
// For each prime number, create one process that:
// 1) reads from its left neighbor over a pipe
// 2) drops all numbers that are divisible by that number
// 3) and writes non-divisible numbers to right neighbor over right pipe.

//NOTE1: Do not forget to close the 4 pointers(2 of each process RW) to the pipe file descriptor
//NOTE2: Maximum number is 35 due to file descriptor limitations of xv6
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define MAX_NUMBER_FILEDESCRIPTORS 35
#define READ 0
#define WRITE 1

void sieve_primes_process(int leftpipe[2])
{
    close(leftpipe[WRITE]); //we only receive
    int new_prime;
    int number_passed_filter;
    int pid;
    int rightpipe[2];

    if (read(leftpipe[READ], &new_prime, sizeof(int)) == 0) //read returns 0 when write-side of pipe closes
    {// If we exit here, it means there is no new prime number and pipe closed
        printf("didnt receive any numbers");
        close(leftpipe[READ]);
        exit(0);
    }
    fprintf(1, "prime %d\n", new_prime);
    pipe(rightpipe);
    pid = fork();
    if(pid != 0) {
        //receive numbers from left process that were not divisible by previous filter and pass non-divisible numbers to right process.
        while (read(leftpipe[READ], &number_passed_filter, sizeof(int)) != 0) 
        {
            if(number_passed_filter % new_prime != 0)// non-divisible by current number, prime suspicious
            {
                write(rightpipe[WRITE], &number_passed_filter, sizeof(int));
            }
        }
        close(leftpipe[READ]);
        close(rightpipe[WRITE]);
        wait(0);
    } else {
        sieve_primes_process(rightpipe);
    }
    exit(0);
        
}

int main(int argc, char *argv[])
{
    // validating input
    if(argc <= 1)
    {    
        fprintf(2, "must provide 1 argument specifying max prime number \n");
        exit(0);
    }
    int primes_range = atoi(argv[1]);
    if(primes_range > MAX_NUMBER_FILEDESCRIPTORS)
    {
        fprintf(1, "limiting prime number to %d", MAX_NUMBER_FILEDESCRIPTORS);
        primes_range = 35;
    } else if (primes_range < 2)
    {
        fprintf(1, "cannot calculate primes of numbers smaller than 2\n");
        exit(0);
    }

    // iterating numbers
    int current_number = 2;
    int pid;
    int discover_primes[2];
    pipe(discover_primes);
    
    pid = fork();
    if (pid != 0)
    {   //parent
        close(discover_primes[READ]);
        for (; current_number <= primes_range; current_number++)
        {
            // flush numbers into prime checker
            write(discover_primes[WRITE], &current_number, sizeof(int));
        }
        close(discover_primes[WRITE]);
        wait(0); //wait for child before close
    } else   
    {   
        sieve_primes_process(discover_primes);
    }

    exit(0);
}