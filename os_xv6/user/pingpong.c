// created by Killerpug
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
//ping pong should perform Inter-Process Communication(IPC) to share a byte between parent and child processes. 

int main(int argc, char *argv[])
{
    int pid;
    char buffer[10];
    int ipc_to_parent[2];
    int ipc_to_child[2];    // create read and write file descriptors for dual communication
    pipe(ipc_to_parent);   
    pipe(ipc_to_child);

    /*
    if(status == -1) //error
    {
        exit(-1);
    }
    status = pipe(ipc_to_child);
    if(status == -1) //error
    {
        exit(-1);
    }
    */


    pid = fork();
    printf("pid:%d\n",pid );

    if(pid == 0)
    {
        read(ipc_to_child[0], buffer, sizeof(buffer));   //receive ping
        printf("child read: %s\n", buffer);
        write(ipc_to_parent[1], "pong", 4);   // write pong
    } else {
        write(ipc_to_child[1], "ping", 4);   // write ping
        read(ipc_to_parent[0], buffer, sizeof(buffer));   //receive pong
        printf("parent received: %s\n", buffer);
    }

    sleep(20);
    exit(0);
}
