#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    int x;

    x = 100;

    int rc = fork();

    if (rc == 0) // Child
    {
        printf("-------------------------------\n");
        printf("Child process (pid:%d)\n", (int) getpid());
        printf("-------------------------------\n");
        printf("Value of X: %d\n", x);
        x = 200;
        printf("New Value of x: %d\n", x);
        
    }
    else if (rc > 0) // Parent
    {
        wait(NULL);
        printf("-------------------------------\n");
        printf("Parent process (pid:%d)\n", (int) getpid());
        printf("-------------------------------\n");
        printf("Value of X: %d\n", x);
        x = 300;
        printf("New Value of x: %d\n", x);
    }
    else // error
    {
        fprintf(stderr, "Error creating process");
    }
}