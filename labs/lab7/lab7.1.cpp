#include <sys/types.h>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

int main(){
    auto pid=fork();
    int status;
    switch (pid){
        case -1:
            perror("ebat'");
            return -1;
        case 0:
             std::cout<<"DAWN\n";
            execl("/home/sbd/child",NULL);
            perror("pizda");
            exit(errno);
            break;
        default:
            std::cout<<"SEX\n";
            wait(&status);
            exit(0);
            break;
    }
}