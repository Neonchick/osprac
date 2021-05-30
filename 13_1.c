#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>


char str[256] = "test/a";
char file[256] = "";
char newStr[256] = "";
char number[256] = "";
char prefNumber[256] = "";

int main() {
  if (mkdir("test",0777)<0) {
    printf("Delete \'test\' directiry before run program\n");
    exit(-1);
  }
  FILE *fp;
  if ((fp = fopen("test/a0","w+")) == NULL) {
    printf("Can\'t create file");
    exit(-1);
  }
  fclose(fp);
  int n = 1;
  while(1) {
    sprintf(number,"%d",n);
    sprintf(prefNumber,"%d",n-1);
    strcpy(newStr,"test/a");
    strcpy(str,"a");
    strcat(newStr,number);
    strcat(str,prefNumber);
    if (symlink(str, newStr)<0) {
      printf("Symlink error\n");
      exit(-1);
    }
    if ((fp=fopen(newStr,"w+")) == NULL) {
      printf("%d\n",n-1);
      exit(-1);
    } else {
      fclose(fp);
      n++;
    }
  }
  return 0;
}
