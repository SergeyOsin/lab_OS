#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

using namespace std;

struct msgbuf1 {
    long mtype;
    char mtext[4096];
};

int main() {
    key_t key = ftok("server.cpp", 'A');
    if (key == -1) {
        cerr << "Не удалось создать ключ";
        return 1;
    }

    int msgid = msgget(key, 0644);
    if (msgid == -1) {
        cerr << "Не удалось подключиться к очереди сообщений сервера";
        return 1;
    }

    msgbuf1 message;
    message.mtype = 1; 

    string filepath;
    cout << "Введите путь к файлу: ";
    cin>>filepath;


    strncpy(message.mtext, filepath.c_str(), 4096);
    message.mtext[4095] = '\0'; 

    if (msgsnd(msgid, &message, strlen(message.mtext) + 1, 0) == -1) {
        cerr << "Не удалось отправить запрос серверу";
        return 1;
    }

    msgbuf1 response;

    if (msgrcv(msgid, &response, sizeof(response.mtext), 2, 0) == -1) {
        cerr << "Не удалось получить ответ от сервера";
        return 1;
    }

    cout << response.mtext << '\n';

    return 0;
}