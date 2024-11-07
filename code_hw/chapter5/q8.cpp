#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>
#define BUFSZE 32

int
main(int argc, char *argv[])
{
    // Setup our pipe
    char buff[BUFSZE];
    int p[2];

    if (pipe(p) < 0)
      exit(1);

    int rc1 = fork();
    if (rc1 < 0) {
        exit(1);
    } else if (rc1 == 0) {
       printf("child1\n");
       close(p[0]);   // This one only writes
       dup2(p[1], 1); // redirect stdout to pipe write
       printf("send this to pipe");
    } else {
        int rc2 = fork();
        if (rc2 < 0) {
          exit(1);
        } else if (rc2 == 0) {
          // Child #2
          printf("child2\n");
          close(p[1]);      
          dup2(p[0], 0);    
          char buff[512];   
          read(STDIN_FILENO, buff, 512); 
          printf("%s", buff);

        } else {
          int wc = waitpid(rc2, NULL, 0);
          printf("parent process");
        }
    }
    return 0;
}