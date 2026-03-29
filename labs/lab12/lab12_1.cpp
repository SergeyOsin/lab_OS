#include <iostream>
#include <pthread.h>

using namespace std;

pthread_mutex_t *mutex;

int main(){
       cout<<"Пользователь №1 запущен\n";
       pthread_mutex_init(mutex,NULL);
       
}
