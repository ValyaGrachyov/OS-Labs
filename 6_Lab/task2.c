#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>


long copy_file(const char *source, const char *destination) {
    int src_fd, dest_fd, n;
    char buffer[4096];
    long bytes_copied = 0;
    struct stat stat_buf;

    src_fd = open(source, O_RDONLY);
    if (src_fd < 0) return -1;

    
    if (fstat(src_fd, &stat_buf) < 0) {
        close(src_fd);
        return -1;
    }

    dest_fd = open(destination, O_WRONLY | O_CREAT, stat_buf.st_mode);
    if (dest_fd < 0) {
        close(src_fd);
        return -1;
    }

    while ((n = read(src_fd, buffer, sizeof(buffer))) > 0) {
        if (write(dest_fd, buffer, n) != n) {
            close(src_fd);
            close(dest_fd);
            return -1;
        }
        bytes_copied += n;
    }

    close(src_fd);
    close(dest_fd);

    
    if (chmod(destination, stat_buf.st_mode) < 0) {
        return -1;
    }

    return bytes_copied;
}




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

             if (access(path2, F_OK) != 0) { 
		if (n > 0) {
		    pid_t pid = fork();
		    if (pid == 0) {
		        long bytes = copy_file(path1, path2);
		        if (bytes >= 0) {
		            printf("PID: %d, Copied: %s, Bytes: %ld\n", getpid(), dir->d_name, bytes);
		        } else {
		            perror("Copy failed");
		        }
		        exit(0);
                    } else if (pid > 0) { 
                        n--;
                        wait(NULL); 
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
