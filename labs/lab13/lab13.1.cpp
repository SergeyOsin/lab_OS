#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstdio>

#define nl cout<<'\n';
using namespace std;

const int COLS=2;
const int COUNTMARKS=4;
const int COUNTSTR=8;

int main(){
       string NameMarks[COUNTSTR][COLS]={
              {"Абрамов","Отлично"},
              {"Базаров", "Хорошо"},
              {"Васильев", "Удовлетворительно"},
              {"Дочников", "Хорошо"},
              {"Городов", "Отлично"},
              {"Елисеев", "Хорошо"},
              {"Компьютеров", "Неудовлетворительно"},
              {"Черепанов", "Удовлетворительно"}
       };
       int fileID=open("results.txt", O_CREAT | O_RDWR | O_TRUNC);
       string str="Результаты сдачи экзамена по ОС: \n";
       write(fileID,str.c_str(),str.size());
       write(fileID, "Фамилия \t Оценка\n", 30);
       for(int i=0;i<COUNTSTR;i++){\
              string row=NameMarks[i][0]+" \t "+NameMarks[i][1]+'\n';
              int len=row.size();
              write(fileID, row.c_str(), len);
       }
       write(fileID,"\n",1);
       string marks[COUNTMARKS]={"Неудовлетворительно", "Удовлетворительно", "Хорошо", "Отлично"};
       for(int i=0;i<COUNTMARKS;i++){
              string currentMark="\nСтуденты с оценкой '" + marks[i]+"'"+":\n";
              write(fileID, currentMark.c_str(), currentMark.size());
              for(int j=0;j<COUNTSTR;j++){
                     if (NameMarks[j][1]==marks[i]){
                            string Surname=NameMarks[j][0]+'\n';
                            write(fileID, Surname.c_str(), Surname.size());
                     }
              }
       }
       close(fileID);
       return 0;
}