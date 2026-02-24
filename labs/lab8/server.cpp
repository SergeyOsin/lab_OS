#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h> 
#include <unistd.h>
#include <string.h>
#include <errno.h>

const char *sc = "server_client";  
const char *cs = "client_server"; 

int sc_d;
int cs_d;
FILE* file_d;

const int len_name = 20;
char file[len_name]{0};

bool running = true;

void handler(int signal) {
    running = false;
}

int main() {
    signal(SIGINT, handler);

    snprintf(file, len_name, "output.%d", getpid());  

    int status = mkfifo(cs, 0666);
    if (status<0){
        perror("Не удалось создать канал");
        exit(EXIT_FAILURE);
    }
    
    status = mkfifo(sc, 0666);
    if (status<0){
        perror("Не удалось создать канал");
        unlink(cs);
        exit(EXIT_FAILURE);
    }

    cs_d = open(cs, O_RDONLY);
    if (cs_d<0){
        perror("Не удалось получить доступ к каналу");
        unlink(sc);
        unlink(cs);
        exit(EXIT_FAILURE);
    }
    sc_d = open(sc, O_WRONLY);
    if (sc_d<0){
        perror("Не удалось получить доступ к каналу");
        close(cs_d);
        unlink(sc);
        unlink(cs);
        exit(EXIT_FAILURE);
    }

    if (!(file_d = fopen(file, "w"))){
        perror("Не удалось открыть файл ");
        close(sc_d);
        close(cs_d);
        fclose(file_d);
        unlink(sc);
        unlink(cs);
        exit(EXIT_FAILURE);
    }
    
    const int len_buf = 200;
    char input_buf[len_buf]{0};
    char output_buf[len_buf]{0};
    int cnt = 0;

    printf("Сервер запущен (PID: %d)\n", getpid());

    while (running) {
        sleep(1);

        ssize_t bytes_read = read(cs_d, input_buf, len_buf);
        if (bytes_read > 0) {
            printf("Получено: %s", input_buf);

            snprintf(output_buf, len_buf, "Ответ сервера на сообщение №%d\n", cnt++);
            write(sc_d, output_buf, strlen(output_buf) + 1);
            printf("Отправлено: %s", output_buf);
            fprintf(file_d, "Получено: %sОтвет: %s", input_buf, output_buf);
            fflush(file_d);
            memset(input_buf, 0, len_buf);
        }
    }
    
    close(sc_d);
    close(cs_d);
    fclose(file_d);
    unlink(sc);
    unlink(cs);
    return 0;
}
