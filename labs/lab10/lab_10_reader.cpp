#include <iostream>
#include <stdio.h>
#include <sys/sem.h>
using namespace std;

int const SEM_IND = 0;
int const SEM_ENTER = -1;
int const SEM_REALESE = 1;
int const CNT_SEM = 1;

union semun {
      int val;                  
      struct semid_ds *buf;     
      unsigned short *array;                         
      struct seminfo *__buf;    
};

char buffer[128];


time_t MAX_SEC = 10;
long MAX_NANO_SEC = 0;

int main(int n, char* argv[]){
       struct sembuf sop;
       sop.sem_num = SEM_IND;  
       sop.sem_op = SEM_ENTER;   
       sop.sem_flg = SEM_UNDO;

       struct timespec *timeOut = new timespec;
       timeOut->tv_sec = MAX_SEC;
       timeOut->tv_nsec = MAX_NANO_SEC;

       key_t key=ftok(argv[1],'A');
       if (key==-1){
              cerr<<"Ошибка ключа\n";
              return 1;
       }

       cout << "Ожидание семафора...\n"; 
       int idSem=semget(key, CNT_SEM, IPC_CREAT | 0666);
       if (idSem == -1) {
              cerr << "Ошибка получения идентификатора\n";
              return 1;
       }

       if (semtimedop(idSem, &sop, CNT_SEM, timeOut) == -1){
              cerr << "Не удалось получить доступ к семафору\n";
              return 1;      
       }

       cout<<"Семафор установлен. Выполняется выполняется чтение файла\n";

       FILE* file=fopen(argv[1],"r");
       if (!file){
              cerr<<"Не удалось открыть файл\n";
              return 1;
       }

       if (fgets(buffer, 128, file) == NULL) {
            cerr << "Файл пуст или ошибка чтения\n";
        } else {
            cout << "Прочитано: " << buffer;
        }
       fclose(file);
       
       sop.sem_op = SEM_REALESE;
       if (semop(idSem, &sop, CNT_SEM) == -1) {
              cerr << "Ошибка снятия семафора\n";
              return 1;
       }
       cout << "Чтение завершено. Семафор cнят.\n";
       semctl(idSem, SEM_IND, IPC_RMID);
       return 0;
}