#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/sem.h>

// Схематично программа имеет следующую логику:
// Логика родителя:
// Запись
// A(2)
// Z()
// Чтение
// Логика ребенка:
// D(1)
// Чтение
// Запись
// D(1)
// Родитель делает запись, делает А(2) и Z() блокируясь пока не вычтут 2
// Ребенок ждет перед чтением первым D(1) и уведомляет о записи вторым D(1)

int main()
{
  int     fd[2], result, n;
  size_t size;
  char  resstring[15];
  int   semid;
  char pathname[]="09-3.c";
  key_t key;
  struct sembuf mybuf;

  if ((key = ftok(pathname,0)) < 0) {
    printf("Can\'t generate key\n");
    exit(-1);
  }

  if ((semid = semget(key, 1, 0666 | IPC_CREAT)) < 0) {
    printf("Can\'t create semaphore set\n");
    exit(-1);
  }

  printf("Write n:\n");
  if (scanf("%d", &n) == 0) {
    printf("Can\'t read n\n");
    exit(-1);
  }

  if (pipe(fd) < 0) {
    printf("Can\'t open pipe\n");
    exit(-1);
  }

  result = fork();

  if (result < 0) {
    printf("Can\'t fork child\n");
    exit(-1);
  } else if (result > 0) {

   /* Parent process */
    for (int i=1; i<=n; i++) {

      size = write(fd[1], "Parent message", 15);

      if (size != 15) {
        printf("Can\'t write all string to pipe\n");
        exit(-1);
      }

      printf("Parent write for the %d time\n", i);

      mybuf.sem_num = 0;
      mybuf.sem_op  = 2;
      mybuf.sem_flg = 0;

      if (semop(semid, &mybuf, 1) < 0) {
        printf("Can\'t add to semaphore\n");
        exit(-1);
      }

      mybuf.sem_num = 0;
      mybuf.sem_op  = 0;
      mybuf.sem_flg = 0;

      if (semop(semid, &mybuf, 1) < 0) {
        printf("Can\'t add to semaphore\n");
        exit(-1);
      }

      size = read(fd[0], resstring, 14);

      if (size < 0) {
        printf("Can\'t read string from pipe\n");
        exit(-1);
      }

      printf("Parent read: \'%s\' for the %d time\n", resstring, i);

    }

    if (close(fd[0]) < 0) {
      printf("parent: Can\'t close reading side of pipe\n");
      exit(-1);
    }

    if (close(fd[1]) < 0) {
      printf("parent: Can\'t close writing side of pipe\n");
      exit(-1);
    }

  } else {

    /* Child process */
    for (int i=1; i<=n; i++) {

      mybuf.sem_num = 0;
      mybuf.sem_op  = -1;
      mybuf.sem_flg = 0;

      if (semop(semid, &mybuf, 1) < 0) {
        printf("Can\'t add to semaphore\n");
        exit(-1);
      }

      size = read(fd[0], resstring, 15);

      if (size < 0) {
        printf("Can\'t read string from pipe\n");
        exit(-1);
      }

      printf("Child read: \'%s\' for the %d time\n", resstring, i);

      size = write(fd[1], "Child message", 14);

      if (size != 14) {
        printf("Can\'t write all string to pipe\n");
        exit(-1);
      }

      printf("Child write for the %d time\n", i);

      mybuf.sem_num = 0;
      mybuf.sem_op  = -1;
      mybuf.sem_flg = 0;

      if (semop(semid, &mybuf, 1) < 0) {
        printf("Can\'t add to semaphore\n");
        exit(-1);
      }

    }

    if (close(fd[0]) < 0) {
      printf("child: Can\'t close reading side of pipe\n"); exit(-1);
    }

    if (close(fd[1]) < 0) {
      printf("child: Can\'t close writing side of pipe\n"); exit(-1);
    }

  }

  return 0;
}
