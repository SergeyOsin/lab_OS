#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

using namespace std;

struct InputData {
    int first;
    int second;
};

 void* thread2(void* arg) {
        cout << "Порожден 2-ой поток: TID - " << gettid() << endl;
        struct InputData* t = (struct InputData*)arg;
        t->first = t->second * 3;
        cout << "Поток №2: x = " << t->first<< ", y = " << t->second << "\n";
        string command="ps -T -p ";
        command+=to_string(getpid());
        system(command.c_str());
        return NULL;
 }
 
void* thread1(void* arg) { 
    cout << "Порожден 1-ый поток: TID - " << gettid() << endl;
    pthread_t Pid;
    struct InputData* t = (struct InputData*)arg;
    for (int i = 0; i < 5; ++i) {  
        cout << "Поток №1: x = " << ++t->first << ", y = " << ++t->second << "\n";
    }
    sleep(2); 
    pthread_create(&Pid, NULL, thread2, t);
    pthread_join(Pid, NULL);
    cout<<"Завершился 2-ой поток: "<<endl;
    string command="ps -T -p ";
    command+=to_string(getpid());
    system(command.c_str());
    return NULL;
}


int main() {
    pthread_t Pid;
    InputData data1 = {0, 0};  

    pthread_create(&Pid, NULL, thread1, &data1);
    pthread_join(Pid, NULL);
    cout<<"Завершился 1-ый поток: \n";
    system("ps");
    cout<<data1.first<<" "<<data1.second<<endl;

    return 0;
}
