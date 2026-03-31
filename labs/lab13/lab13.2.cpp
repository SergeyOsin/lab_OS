#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

using namespace std;

int main(){
       cout<<"Введите путь к файлу: ";
       string pathFile;
       cin>>pathFile;
       const char* File=pathFile.c_str();
       int fileID = open(File,O_RDONLY);
       if (!(fileID+1)){
              cerr<<"Указанного файла не существует";
              close(fileID);
              return 1;
       }
       struct stat StatFile;
       int StatID=fstat(fileID, &StatFile);
       if (StatID < 0) {
              cerr << "Не удалось получить атрибуты\n";
              close(fileID);
              return 1;
       }
       cout<<"Атрибуты разрешения файла: ";
       string atrbs;
       atrbs+=(StatFile.st_mode & S_IRUSR)?'R':'-';
       atrbs+=(StatFile.st_mode & S_IWUSR)?'W':'-';
       atrbs+=(StatFile.st_mode & S_IXUSR)?'X':'-';
       cout<<atrbs<<'\n';
       if (atrbs.back()=='X')
              cout<<"Указанный файл уже имеет разрешение на исполнение\n";
       else{
              fchmod(fileID, StatFile.st_mode | S_IXUSR);
              cout<<"Установлен атрибут для исполнения файла";
       }
       close(fileID);
       return 0;
}