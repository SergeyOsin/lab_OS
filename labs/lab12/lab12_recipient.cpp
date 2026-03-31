#include <iostream>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <pthread.h>
#include "Mailbox.hpp"

using namespace std;

struct arguments{
    string file_name;
};

pthread_mutex_t mutex;
int *value;

void* FileWork(void* arg) {
    arguments* data = (arguments*)arg;

    pthread_mutex_lock(&mutex);

    FILE* File = fopen(data->file_name.c_str(), "a+");
    if (!File) {
        perror("Не удалось открыть файл");
        pthread_mutex_unlock(&mutex);
        return NULL;
    }

    rewind(File);
    char line[4096];
    while (fgets(line, sizeof(line), File)) {
        cout << line << endl;
    }

  
    pid_t tid = gettid();
    string mess = "Привет, написал это - " + to_string(tid) + ", значение переменной - " + to_string(*value) +"\n";

    *value = 4; 
    fwrite(mess.c_str(), 1, mess.size(), File);

    fclose(File);
    pthread_mutex_unlock(&mutex);
    return NULL;
}

int main(int argc, char* argv[]) {
    pthread_t Pid_first;
    pthread_t Pid_second;
    value = new int;
    *value = 3;
    pthread_mutex_init(&mutex, NULL);

    arguments args = {argv[1]};  

    pthread_create(&Pid_first, NULL, FileWork, &args);
    pthread_create(&Pid_second, NULL, FileWork, &args);

    pthread_join(Pid_first, NULL);
    pthread_join(Pid_second, NULL);


    return 0;
}

