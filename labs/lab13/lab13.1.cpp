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

int main(){
       cout<<"Введите количество студентов: ";
       int NumberStudents; cin>>NumberStudents;
       const int COUNTSTR=NumberStudents;
       string NameMarks[COUNTSTR][COLS];

       int fileID=open("results.txt", O_CREAT | O_RDWR | O_TRUNC);
       string str="Результаты сдачи экзамена по ОС: \n";
       write(fileID,str.c_str(),str.size());
       write(fileID, "Фамилия \t Оценка\n", 30);

       cout<<"Фамилия и оценки студентов \n";
       for(int i=0;i<COUNTSTR;i++){
              cout<<"Введите фамилию студента: ";
              cin>>NameMarks[i][0];
              cout<<"Введите оценку студента (2,3,4,5): ";
              cin>>NameMarks[i][1];
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
                     if (NameMarks[j][1]==to_string(i+2)){
                            string Surname=NameMarks[j][0]+'\n';
                            write(fileID, Surname.c_str(), Surname.size());
                     }
              }
       }
       cout<<"\n Файл results.txt \n";
       close(fileID);
       return 0;
}