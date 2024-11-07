#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t pid;
    int status;

    // Fork the process
    pid = fork();

    if (pid == -1) {
        // Error forking
        perror("fork failed");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // Child process: exec different variants to run /bin/ls

        // Uncomment the exec variant you want to try:

        // Variant 1: execl() - Pass path and arguments one by one
        // execl("/bin/ls", "ls", "-l", (char *) NULL);

        // Variant 2: execle() - Pass path, arguments, and custom environment
        char *envp[] = { "PATH=/bin", (char *) NULL };
        execle("/bin/ls", "ls", "-l", (char *) NULL, envp);

        // Variant 3: execlp() - Search for the program in the PATH environment variable
        // execlp("ls", "ls", "-l", (char *) NULL);

        // Variant 4: execv() - Pass path and arguments as an array
        // char *args[] = { "ls", "-l", NULL };
        // execv("/bin/ls", args);

        // Variant 5: execvp() - Search for the program in the PATH environment variable, using args array
        // char *args[] = { "ls", "-l", NULL };
        // execvp("ls", args);

        // Variant 6: execvpe() - Pass program, arguments, and custom environment
        // char *args[] = { "ls", "-l", NULL };
        // char *envp[] = { "PATH=/bin", (char *) NULL };
        // execvpe("ls", args, envp);

        // If exec fails
        perror("exec failed");
        exit(EXIT_FAILURE);
    } else {
        // Parent process: wait for child to finish
        waitpid(pid, &status, 0);
        if (WIFEXITED(status)) {
            printf("Child process exited with status %d\n", WEXITSTATUS(status));
        } else {
            printf("Child process did not exit successfully\n");
        }
    }

    return 0;
}
