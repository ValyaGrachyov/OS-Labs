#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

typedef struct {
    char *path;
    char *name;
    off_t size;
} FileInfo;

void exploreDirectory(const char *dirPath, FileInfo **files, int *fileCount) {
    DIR *dir = opendir(dirPath);
    if (dir == NULL) {
        perror("Fail when open catalog");
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR) {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;

            char subDirPath[PATH_MAX];
            snprintf(subDirPath, PATH_MAX, "%s/%s", dirPath, entry->d_name);
            exploreDirectory(subDirPath, files, fileCount);
        } else if (entry->d_type == DT_REG) {
            char filePath[PATH_MAX];
            snprintf(filePath, PATH_MAX, "%s/%s", dirPath, entry->d_name);

            struct stat fileStat;
            if (stat(filePath, &fileStat) == -1) {
                perror("Fail while get info about file");
                continue;
            }

            *files = realloc(*files, (*fileCount + 1) * sizeof(FileInfo));
            (*files)[*fileCount].path = strdup(filePath);
            (*files)[*fileCount].name = strdup(entry->d_name);
            (*files)[*fileCount].size = fileStat.st_size;
            (*fileCount)++;
        }
    }
    closedir(dir);
}

int compareBySize(const void *a, const void *b) {
    FileInfo *fileA = (FileInfo *)a;
    FileInfo *fileB = (FileInfo *)b;
    return (fileA->size > fileB->size) - (fileA->size < fileB->size);
}

int compareByName(const void *a, const void *b) {
    FileInfo *fileA = (FileInfo *)a;
    FileInfo *fileB = (FileInfo *)b;
    return strcmp(fileA->name, fileB->name);
}

void copyFile(const char *source, const char *destination) {
    FILE *src = fopen(source, "rb");
    if (src == NULL) {
        perror("Fail src file");
        return;
    }

    FILE *dest = fopen(destination, "wb");
    if (dest == NULL) {
        perror("Fail dest file");
        fclose(src);
        return;
    }

    char buffer;
    while ((buffer = fgetc(src)) != EOF) {
        fputc(buffer, dest);
    }

    fclose(src);
    fclose(dest);
}


void copyFilesFromDir(const char *srcDirPath, const char *destDirPath) {
    DIR *dir = opendir(srcDirPath);
    if (dir == NULL) {
        perror("Fail src catalog");
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) {
            char srcFilePath[PATH_MAX];
            snprintf(srcFilePath, PATH_MAX, "%s/%s", srcDirPath, entry->d_name);

            char destFilePath[PATH_MAX];
            snprintf(destFilePath, PATH_MAX, "%s/%s", destDirPath, entry->d_name);

            copyFile(srcFilePath, destFilePath);
        }
    }

    closedir(dir);
}


void printFileInfo(const FileInfo *fileInfo) {
    printf("Path: %s, Name: %s, Size: %ld byte\n", fileInfo->path, fileInfo->name, fileInfo->size);
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Invalid args \n");
        return 1;
    }

    int sortCriterion = atoi(argv[2]);
    FileInfo *files = NULL;
    int fileCount = 0;

    exploreDirectory(argv[1], &files, &fileCount);

    if (sortCriterion == 1) {
        qsort(files, fileCount, sizeof(FileInfo), compareBySize);
    } else if (sortCriterion == 2) {
        qsort(files, fileCount, sizeof(FileInfo), compareByName);
    } else {
        fprintf(stderr, "Invalid sort criterion\n");
        return 1;
    }

    for (int i = 0; i < fileCount; i++) {
       
        mkdir(argv[3], 0755);
        copyFilesFromDir(argv[1], argv[3]);
        printFileInfo(&files[i]);
    }

    for (int i = 0; i < fileCount; i++) {
        free(files[i].path);
        free(files[i].name);
    }
    free(files);

    return 0;
}



