#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
    FILE *srcFile, *destFile;
    int ch;
    struct stat fileStat;

    if (argc != 3) {
        fprintf(stderr, "Invalid args\n");
        return 1;
    }

    srcFile = fopen(argv[1], "r");
    if (srcFile == NULL) {
        perror("Fail to open source file");
        return 1;
    }

    
    if (stat(argv[1], &fileStat) != 0) {
        perror("Fail to get info about file");
        fclose(srcFile);
        return 1;
    }

    destFile = fopen(argv[2], "w");
    if (destFile == NULL) {
        perror("Fail to open src file");
        fclose(srcFile);
        return 1;
    }

    while ((ch = fgetc(srcFile)) != EOF) {
        if (fputc(ch, destFile) == EOF) {
            perror("Fail to write into file");
            fclose(srcFile);
            fclose(destFile);
            return 1;
        }
    }

    if (ferror(srcFile)) {
        perror("Fail to read src file");
        fclose(srcFile);
        fclose(destFile);
        return 1;
    }

    fclose(srcFile);
    fclose(destFile);

    if (chmod(argv[2], fileStat.st_mode) != 0) {
        perror("Fail to set access to file");
        return 1;
    }

    return 0;
}

