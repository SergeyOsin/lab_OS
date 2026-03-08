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

    cs_d = open(cs, O_WRONLY);

    if (cs_d == -1) {
        perror("Не удалось получить доступ к каналу");
        exit(EXIT_FAILURE);
    }

    sc_d = open(sc, O_RDONLY);

    if (sc_d == -1) {
        perror("Не удалось получить доступ к каналу");
        close(cs_d);
        exit(EXIT_FAILURE);
    }



    if(! (file_d = fopen(file, "w"))){
        perror("Не удалось открыть файл");
        close(sc_d);
        close(cs_d);
        fclose(file_d);
        exit(EXIT_FAILURE);
    }

    const int len_buf = 200;
    char input_buf[len_buf]{0};
    char output_buf[len_buf]{0};
    int cnt = 0;

    printf("Клиент запущен (PID: %d)\n", getpid());

    while (running) {
        sleep(1);

        snprintf(output_buf, len_buf, "Сообщение от клиента №%d\n", cnt++);
        write(cs_d, output_buf, strlen(output_buf) + 1);
        printf("Отправлено: %s", output_buf);

        ssize_t bytes_read = read(sc_d, input_buf, len_buf);
        if (bytes_read > 0) {
            printf("Получено: %s", input_buf);
            fprintf(file_d, "Получено: %s", input_buf);
            fflush(file_d);
            memset(input_buf, 0, len_buf);
        }
    }

    close(sc_d);
    close(cs_d);
    fclose(file_d);
    return 0;
}
