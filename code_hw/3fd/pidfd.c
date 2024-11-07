#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <signal.h>
#include <sys/wait.h>
#include <errno.h>

int main(void) {
    pid_t child_pid;
    int pidfd;

    child_pid = fork();
    if (child_pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (child_pid == 0) {
        printf("Child PID %d waiting for signal...\n", getpid());
        pause();
        printf("Child received signal, exiting.\n");
        exit(EXIT_SUCCESS);
    } else {
        sleep(1);

        pidfd = syscall(SYS_pidfd_open, child_pid, 0);
        if (pidfd == -1) {
            perror("pidfd_open");
            exit(EXIT_FAILURE);
        }

        if (syscall(SYS_pidfd_send_signal, pidfd, SIGTERM, NULL, 0) == -1) {
            perror("pidfd_send_signal");
            exit(EXIT_FAILURE);
        }
        printf("Parent sent SIGTERM to child via pidfd.\n");

        close(pidfd);
        wait(NULL); // Wait for child to exit
        printf("Child has exited, parent exiting.\n");
    }

    return 0;
}
