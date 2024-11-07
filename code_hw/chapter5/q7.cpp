#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    int rc = fork();

    if (rc == 0) // child process
    {
        close(STDOUT_FILENO);
        printf("nobody see this\n");
    }
    else if (rc > 0)
    {
        wait(NULL);
        printf("parent process\n");
    }
    else 
    {
        printf("Error in fork()\n");
    }
}