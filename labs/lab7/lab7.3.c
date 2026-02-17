#define _POSIX_C_SOURCE 200809L 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

int pid;


void handler(int sig, siginfo_t* info, void *) {
    printf("Завершен процесс PID: %d, PPID %d\n", info->si_pid, getpid());
}

void bornProcess(int cnt){
    if (cnt > 0){
        pid = fork();
        if (pid != 0){
            bornProcess(--cnt);
        }
        else{
            printf("Порожден процесс PID: %d, PPID %d\n", getpid(), getppid());
        }
        
    }
}

int main() {
        struct sigaction sa = {0};
        sigset_t sigset;
        sigemptyset(&sigset);
        sigaddset(&sigset, SIGUSR2);
        sa.sa_sigaction = handler;
        sa.sa_flags = SA_SIGINFO;
        sigaction(SIGUSR2, &sa, NULL);
        bornProcess(7);
        if (pid == 0){
            kill(getppid(), SIGUSR2);
        }
        else{
            while(1);
        }
    return 0;
    }