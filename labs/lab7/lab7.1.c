#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int var = 1;

int main() {
    FILE *fp = fopen("result.txt", "w");
    if (!fp) exit(1);
    dup2(fileno(fp), STDOUT_FILENO);
    fclose(fp);
    
    printf("До разветвления\n");
    printf("Значение var: %d\n", var++);
    printf("Значение var++: %d\n", var);

    fflush(stdout);
    
    pid_t pid = fork();
    if (pid < 0) exit(1);

    if (pid == 0) {
        printf("\nДочерний процесс\n");
        printf("Значение var: %d\n", var);
        printf("Значение var+5: %d\n", var+=5);
        exit(0);
    } else {
        wait(NULL);
        printf("\nРодительский процесс\n");
        printf("Значение var: %d\n", var);
        printf("Значение var+3: %d\n", var+=3);
    }    

    return 0;
}
