#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

int size = 0;

void action() {
  printf("%d bytes\n",size);
  exit(0);
}

int main()
{
  int     fd[2];
  int     writeSize;
  char    ch = '1';

  if (pipe(fd) < 0) {
    printf("Can\'t open pipe\n");
    exit(-1);
  }

  signal(SIGALRM, action);

  do {
    alarm(1);
    writeSize = write(fd[1], &ch ,1);
    if (writeSize < 0) {
      printf("Can't write to pipe\n"); exit(-1);
    }
    size++;
    alarm(0);
  } while (1);

  if (close(fd[0]) < 0) {
    printf("Can'\t close reading side of pipe\n"); exit(-1);
  }
  if (close(fd[1]) < 0) {
    printf("Can'\t close writing side of  pipe\n"); exit(-1);
  }

  return 0;
}
