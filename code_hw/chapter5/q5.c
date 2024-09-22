#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    int rc = fork();

    if (rc == 0) // child process
    {
        int wc = wait(NULL);
        printf("child process\n");
        printf("Return code from wait() is %d\n", wc);
        printf("-------------------------------------\n");
    }
    else if (rc > 0) // parent process
    {
        int wc = wait(NULL);
        printf("parent process\n");
        printf("Return code from wait() is %d\n", wc);
        printf("-------------------------------------\n");
    }
    else
    {
        fprintf(stderr, "Error from fork()");
    }
}