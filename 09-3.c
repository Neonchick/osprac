#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/sem.h>

// Схематично программа имеет следующую логику:
// Логика родителя:
// D(1)
// Запись
// A(2)
// D(3)
// Чтение 
// А(1)
// Логика ребенка:
// D(2)
// Чтение
// Запись
// А(3)
// В начале семафор инициализируется 1
// Дальше D(1) родителя заканчивает блокировку и родитель начинает запись
// Дальше родитель делает A(2), чтобы ребенок разблокировался от D(2) и начал чтение и запись
// Дальше ребенок делает А(3), чтобы родитель разблокировался от D(3) и начал чтение
// Дальше родитель делает А(1), чтобы родитель смог корректно начать следующую итерацию


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

  if ((semid = semget(key, 1, 0666 | IPC_CREAT | IPC_EXCL)) < 0) {
    if (errno != EEXIST) {
      printf("Can\'t create semaphore set\n");
      exit(-1);
    } else {
      if ((semid = semget(key, 1, 0666 | IPC_CREAT)) < 0) {
        printf("Can\'t find semaphore set\n");
        exit(-1);
      }
    }
  } else {
      mybuf.sem_num = 0;
      mybuf.sem_op  = 1;
      mybuf.sem_flg = 0;
      if (semop(semid, &mybuf, 1) < 0) {
        printf("Can\'t change semaphores\n");
        exit(-1);
      }
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

      mybuf.sem_num = 0;
      mybuf.sem_op  = -1;
      mybuf.sem_flg = 0;

      if (semop(semid, &mybuf, 1) < 0) {
        printf("Can\'t add to semaphore\n");
        exit(-1);
      }

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
      mybuf.sem_op  = -3;
      mybuf.sem_flg = 0;

      if (semop(semid, &mybuf, 1) < 0) {
        printf("Can\'t minus to semaphore\n");
        exit(-1);
      }

      size = read(fd[0], resstring, 14);

      if (size < 0) {
        printf("Can\'t read string from pipe\n");
        exit(-1);
      }

      printf("Parent read: \'%s\' for the %d time\n", resstring, i);

      mybuf.sem_num = 0;
      mybuf.sem_op  = 1;
      mybuf.sem_flg = 0;

      if (semop(semid, &mybuf, 1) < 0) {
        printf("Can\'t minus to semaphore\n");
        exit(-1);
      }

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
      mybuf.sem_op  = -2;
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
      mybuf.sem_op  = 3;
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
