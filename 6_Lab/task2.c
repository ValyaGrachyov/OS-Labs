#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/wait.h>
#include <string.h>

int main() {
    char dir1[100], dir2[100];
    int n;

    printf("Enter directory 1: ");
    scanf("%s", dir1);
    printf("Enter directory 2: ");
    scanf("%s", dir2);
    printf("Enter max number of processes: ");
    scanf("%d", &n);

    DIR *d1;
    struct dirent *dir;
    d1 = opendir(dir1);

    if (d1) {
        while ((dir = readdir(d1)) != NULL) {
            if (!strcmp(dir->d_name, ".") || !strcmp(dir->d_name, ".."))
                continue;

            char path1[200], path2[200];
            sprintf(path1, "%s/%s", dir1, dir->d_name);
            sprintf(path2, "%s/%s", dir2, dir->d_name);

            if (access(path2, F_OK) != 0) { // File does not exist in dir2
                if (n > 0) {
                    pid_t pid = fork();

                    if (pid == 0) { // Child process
                        printf("PID: %d, Copying: %s\n", getpid(), dir->d_name);
                        execlp("cp", "cp", "--preserve=all", path1, path2, NULL);
                        exit(0);
                    } else if (pid > 0) { // Parent process
                        n--;
                        wait(NULL); // Wait for a child to finish
                        n++;
                    } else {
                        perror("fork failed");
                        exit(1);
                    }
                }
            }
        }
        closedir(d1);
    }

    return 0;
}

