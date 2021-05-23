#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
  int msqid;
  long pid = getpid() + 1;
  char pathname[]="12_server.c"; // The file name used to generate the key.
                             // A file with this name must exist in the current directory.
  key_t  key;     // IPC key
  int i,len, maxlen;      // Cycle counter and the length of the informative part of the message

  struct mymsgbuf // Custom structure for the message
  {
    long mtype;
    struct {
      long pid;
      double number;
    } info;
  } mybuf, mybuf2;

  if ((key = ftok(pathname,0)) < 0) {
    printf("Can\'t generate key\n");
    exit(-1);
  }
  //
  // Trying to get access by key to the message queue, if it exists,
  // or create it, with read & write access for all users.
  //
  if ((msqid = msgget(key, 0666)) < 0) {
    printf("Can\'t find server\n");
    exit(-1);
  }

  /* Send information */

  int continueInt;

  mybuf.mtype = 2;
  len = sizeof(mybuf.info);

  do {
    printf("Enter double\n");
    scanf("%lf", &mybuf.info.number);

    mybuf.info.pid = pid;

    len = sizeof(mybuf.info);
    // Send the message. If there is an error,
    // report it and delete the message queue from the system.
    //
    if (msgsnd(msqid, (struct msgbuf *) &mybuf, len, 0) < 0) {
      printf("Can\'t send message to queue\n");
      msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
      exit(-1);
    }

    maxlen = sizeof(mybuf2.info);

    if (( len = msgrcv(msqid, (struct msgbuf *) &mybuf2, maxlen, pid, 0)) < 0) {
      printf("Can\'t receive message from queue\n");
      msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
      exit(-1);
    }

    printf("Server ans %lf\n",mybuf2.info.number);
    printf("If you want enter another double enter 1 or another number to exit\n");
    scanf("%d", &continueInt);
  } while (continueInt == 1);

  return 0;
}
