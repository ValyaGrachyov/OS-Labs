#include <stdio.h>
#include <stdlib.h>

void waitForKeyPress() {
    printf("Any button for continue\n");
    getchar();
}

int main(int argc, char *argv[]) {
    FILE *file;
    int ch, lineCount = 0, linesPerGroup;

    if (argc != 3) {
        fprintf(stderr, "Use as argument filename and count of rows\n");
        return 1;
    }

    file = fopen(argv[1], "r");
    if (file == NULL) {
        perror("Fail to open file");
        return 1;
    }

    linesPerGroup = atoi(argv[2]);

    while ((ch = fgetc(file)) != EOF) {
        putchar(ch);
        if (ch == '\n') {
            lineCount++;
            if (linesPerGroup != 0 && lineCount == linesPerGroup) {
                waitForKeyPress();
                lineCount = 0;
            }
        }
    }

    if (ferror(file)) {
        perror("Fail to read file");
        fclose(file);
        return 1;
    }

    fclose(file);
    return 0;
}

