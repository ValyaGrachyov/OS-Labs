#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>

void listDirectory(const char *dirPath) {
    DIR *dir;
    struct dirent *entry;

    dir = opendir(dirPath);
    if (dir == NULL) {
        perror("Fail to open catalog");
        return;
    }

    printf("Catalog include %s:\n", dirPath);
    while ((entry = readdir(dir)) != NULL) {
        printf("%s\n", entry->d_name);
    }

    closedir(dir);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Invalid arguments");
        return 1;
    }

   
    listDirectory(".");

    
    listDirectory(argv[1]);

    return 0;
}

