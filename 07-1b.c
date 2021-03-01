#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

int main()
{
  int     i;
  int     *array;
  int     shmid;
  int     new = 1;
  char    pathname[] = "07-1a.c";
  key_t   key;

  if ((key = ftok(pathname,0)) < 0) {
    printf("Can\'t generate key\n");
    exit(-1);
  }

  if ((shmid = shmget(key, 4096*sizeof(int), 0)) < 0) {
     printf("Can\'t find shared memory\n");
     exit(-1);
  }

  if ((array = (int *)shmat(shmid, NULL, 0)) == (int *)(-1)) {
    printf("Can't attach shared memory\n");
    exit(-1);
  }

  i = 0;
  printf("07-1a.c:\n\n");
  while (array[i] != EOF) {
    putchar(array[i]);
    i++;
  }

  if (shmctl(shmid, IPC_RMID, NULL) < 0) {
    printf("Can't free shared memory\n");
    exit(-1);
  }

  printf("Shared memory released\n");

  return 0;
}
