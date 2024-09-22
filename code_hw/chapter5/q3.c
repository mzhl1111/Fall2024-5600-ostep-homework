#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define true 1
#define false 1
int main()
{
    int rc = fork();

    if (rc == 0) // child process
    {
        printf("hello\n");
        exit(0);
    }
    else if (rc > 0)
    {
        wait(NULL);
        printf("goodbye\n");
    }
    return 0;
}