#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

int main()
{
  int     fd, readResult;
  size_t  size;
  char    ch;

  if ((fd = open("myfile", O_RDONLY)) < 0) {
    printf("Can\'t open file\n");
    exit(-1);
  }

  readResult = read(fd, &ch, 1);
  while (readResult > 0) {
    printf("%c",ch);
    readResult = read(fd, &ch, 1);
  }

  if (readResult < 0) {
    printf("Can\'t read file\n");
    exit(-1);
  }

  if (close(fd) < 0) {
    printf("Can\'t close file\n");
  }

  return 0;
}
