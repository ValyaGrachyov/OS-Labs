#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

/*
 * Написать программу, создающую два дочерних процесса с использованием
 * двух вызовов fork(). Родительский и два дочерних процесса должны выводить на
 * экран свой pid и pid родительского процесса и текущее время
 * в формате: часы : минуты : секунды : миллисекунды. Используя вызов system(),
 * выполнить команду ps -x в родительском процессе. Найти свои процессы в списке запущенных процессов.
 */

void get_pid(pid_t pid) {
    int hours, minutes, seconds;
    time_t now;
    time(&now);
    struct tm *local = localtime(&now);
    struct timespec start;
    clock_gettime(CLOCK_MONOTONIC, &start);
    long mili = start.tv_nsec / 100000;

    hours = local->tm_hour;
    minutes = local->tm_min;
    seconds = local->tm_sec;

    if (pid == 0) {
        printf("Это Дочерний процесс, его pid=%d time: %d:%d:%d:%ld\n", getpid(), hours, minutes, seconds, mili);
        printf("А pid его Родительского процесса=%d time: %d:%d:%d:%ld\n", getppid(), hours, minutes, seconds, mili);
    } else if (pid > 0)
        printf("Это Родительский процесс pid=%d time: %d:%d:%d:%ld\n", getpid(), hours, minutes, seconds, mili);
    else {
        printf("Ошибка вызова fork, потомок не создан\n");
        return;
    }

}

int main() {
    pid_t pid_1;
    pid_t pid_2;

    pid_1 = fork();

    if (pid_1 > 0) {
        pid_2 = fork();
        get_pid(pid_2);
        if (pid_2 > 0) {
            system("ps -x | grep 'task1.exe'");
        }
    }

    return 0;
}

