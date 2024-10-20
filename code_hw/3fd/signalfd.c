#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/signalfd.h>
#include <signal.h>
#include <string.h>
#include <bits/sigaction.h>

int main() {
    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGINT);  
    sigprocmask(SIG_BLOCK, &mask, NULL);  

    int sfd = signalfd(-1, &mask, SFD_NONBLOCK);
    if (sfd == -1) {
        perror("signalfd");
        exit(EXIT_FAILURE);
    }

    struct signalfd_siginfo fdsi;
    while (1) {
        ssize_t s = read(sfd, &fdsi, sizeof(fdsi));
        if (s == sizeof(fdsi)) {
            if (fdsi.ssi_signo == SIGINT) {
                printf("Received SIGINT\n");
                break;
            }
        }
    }

    close(sfd);
    return 0;
}
