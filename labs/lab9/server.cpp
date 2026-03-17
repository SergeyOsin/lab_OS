#include <iostream>
#include <fstream>
#include <cstring>
#include <unistd.h>
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

        if (access(filepath.c_str(),F_OK)!=0) {
            string error = "Файл не существует";
            strncpy(response.mtext, error.c_str(), MAXLEN);
            response.mtext[MAXLEN - 1] = '\0';
        } 
        else if (access(filepath.c_str(),R_OK)!=0){
            string error = "Файл не доступен для чтения";
            strncpy(response.mtext, error.c_str(), MAXLEN);
            response.mtext[MAXLEN - 1] = '\0';
        }
        else {
            string content, line;

            while (getline(file,line))
                content+=line+'\n';

            if (!content.empty()) content.pop_back();

            strncpy(response.mtext, content.c_str(), MAXLEN);
            response.mtext[MAXLEN - 1] = '\0';
            file.close();
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