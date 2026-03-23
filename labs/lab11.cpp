#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

using namespace std;

struct Thread {
    int x;
    int y;
};

void* threads1(void* arg) {
    struct Thread* t = (struct Thread*)arg;
    for (int i = 0; i < 5; ++i) {  
        cout << "Поток №1: x = " << t->x << ", y = " << t->y << "\n";
        sleep(1);  
    }
    return NULL;
}

void* thread2(void* arg) {
    struct Thread* t = (struct Thread*)arg;
    t->y = 25;
    t->x = t->y * 3;
    cout << "Поток №2: x = " << t->x << ", y = " << t->y << "\n";
    return NULL;
}

int main() {
    pthread_t Pid, PID1;
    Thread data1 = {0, 0};  
    Thread data2 = {0, 0}; 

    pthread_create(&Pid, NULL, threads1, &data1);
    sleep(2);
    system("ps");

    pthread_create(&PID1, NULL, thread2, &data2);
    system("ps");

    pthread_join(Pid, NULL);
    pthread_join(PID1, NULL);

    system("ps");
    cout << "Все потоки завершены.\n";

    return 0;
}
