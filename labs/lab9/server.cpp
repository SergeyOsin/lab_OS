#include <iostream>
#include <fstream>
#include <cstring>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

using namespace std;

const int MAXLEN = 4096;

struct msgbuf1 {
    long mtype;
    char mtext[MAXLEN];
};

int main() {
    key_t key = ftok("server.cpp", 'A');
    if (key == -1) {
        cerr << "Не удалось создать ключ";
        return 1;
    }

    int msgid = msgget(key, IPC_CREAT | 0644);
    if (msgid == -1) {
        cerr << "Не удалось получить идентификатор очереди сообщений";
        return 1;
    }

    cout << "Запуск сервера\n";

    msgbuf1 message;

    while (true) {
        if (msgrcv(msgid, &message, sizeof(message.mtext), 1, 0) == -1) {
            cerr << "Ошибка при получении запроса от клиента" << endl;
            break;
        }

        string filepath(message.mtext);
        cout << "Получен запрос на файл: " << filepath << endl;

        msgbuf1 response;
        response.mtype = 2; 

        ifstream file(filepath);

        if (!file.is_open()) {
            string error = "Не удается открыть файл: " + filepath + "' — файл не существует или нет прав на чтение";
            strncpy(response.mtext, error.c_str(), MAXLEN);
            response.mtext[MAXLEN - 1] = '\0';
        } else {
            string content((istreambuf_iterator<char>(file)),
                         istreambuf_iterator<char>());


            strncpy(response.mtext, content.c_str(), MAXLEN);
            response.mtext[MAXLEN - 1] = '\0';
        }

        if (msgsnd(msgid, &response, strlen(response.mtext) + 1, 0) == -1) {
            cerr << "Ошибка при отправке ответа клиенту" << endl;
            break;
        }

        cout << "Ответ отправлен клиенту" << endl;
    }

    msgctl(msgid, IPC_RMID, nullptr);
    return 0;
}
