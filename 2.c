#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
 FILE *file;
 int ch;

 if(argc !=2) {
 fprintf(stderr, "Use as argument filename\n");
 exit(EXIT_FAILURE);
}
 
 file = fopen(argv[1],"w");
 if(file == NULL) {
 perror("Fail to open file\n");
 exit(EXIT_FAILURE);
}

 printf("Write letters (CTRL-F for finish)\n");
 while((ch= getc(stdin)) != EOF) {
 if(ch ==6){ break;}

 if(fputc(ch, file) == EOF) {
 perror("Fail to write in file\n");
 exit(EXIT_FAILURE);
}
}

 if(fclose(file) == EOF) {
 perror("Fail to close file");
 exit(EXIT_FAILURE);
}
 return 0;


}

