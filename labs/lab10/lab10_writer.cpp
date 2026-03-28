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

char message[128];

int main(int n, char* argv[]){
       struct sembuf sop;
       sop.sem_num = SEM_IND;  
       sop.sem_op = SEM_ENTER;   
       sop.sem_flg = SEM_UNDO;

       union semun args;
       args.val = 1;
       key_t key = ftok(argv[1],'A');
       if (key==-1){
              cerr<<"Ошибка ключа\n";
              return 1;
       }

       int idSem = semget(key, CNT_SEM, IPC_CREAT | 0666);
       if (idSem == -1) {
              cerr << "Ошибка получения идентификатора\n";
              return 1;
       }

       int semValue = semctl(idSem, SEM_IND, SETVAL, args);
       if (semValue == -1){
              cerr << "Ошибка инициализации\n";
              return 1;
       }

       semop(idSem, &sop, CNT_SEM);
       cout<<"Семафор установлен. Выполняется запись в файл\n";

       FILE* file = fopen(argv[1],"w");
       if (!file){
              cerr<<"Не удалось открыть файл\n";
              return 1;
       }

       cout << "Введите сообщение: ";
       cin.ignore();  
       cin.getline(message, 128);  
       fprintf(file,"%s\n", message);
       fclose(file);
       

       sop.sem_op = SEM_REALESE;
       if (semop(idSem, &sop, CNT_SEM) == -1) {
              cerr << "Ошибка снятия семафора\n";
              return 1;
       }
       cout << "Запись завершена. Семафор cнят.\n";
       return 0;
}