#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

using namespace std;


struct Thread{
       int x;
       int y;
};

void *threads1(void *arg){
       struct Thread* t=(struct Thread*) arg;
       t->x=4;
       t->y=t->x*3+1;
}

void *thread2(void *arg){
       struct Thread* t=(struct Thread*) arg;
       t->y=25;
       t->x=t->y*3;
}

int main(){
       pthread_t Pid;
      pthread_create(&Pid,NULL,threads1,NULL);
      sleep(2);
      system("ps");
      pthread_t PID1;
      pthread_create(&PID1,NULL,thread2,NULL);
      system("ps");
      pthread_exit(&PID1);
      system("ps");
      pthread_exit(&Pid);
      system("ps");
      return 0; 
}