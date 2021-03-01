#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
  int     fd, result;

  size_t  size;
  char    resstring[14];
  char    name[] = "aaa.fifo";

  printf("Writer exist. Wating for reader.\n");

  if ((fd = open(name, O_WRONLY)) < 0) {
    if (mknod(name, S_IFIFO | 0666, 0) < 0) {
      printf("Can\'t create FIFO\n");
      exit(-1);
    }

    if ((fd = open(name, O_WRONLY)) < 0) {
      printf("Can\'t open FIFO for writting\n");
      exit(-1);
    }
  }

  size = write(fd, "Hello, world!", 14);

  printf("Text writed\n");

  if (size != 14) {
    printf("Can\'t write all string to FIFO\n");
    exit(-1);
  }

  if (close(fd) < 0) {
    printf("parent: Can\'t close FIFO\n"); exit(-1);
  }

  return 0;
}
