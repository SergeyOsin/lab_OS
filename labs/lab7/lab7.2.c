#include <signal.h>
#include <stdio.h>


int const SECOND_LIMIT = 1;
int current_second = 0;

void handler(){
    printf("Программа работает на протяжении: %d секунд\n", current_second);
    current_second++;
    alarm(SECOND_LIMIT);
}

int main(){
    signal(SIGALRM, handler);
    alarm(SECOND_LIMIT);
    while(1){
        pause();
    }
}