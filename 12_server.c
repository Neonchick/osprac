#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int main(void)
{
  int msqid;
  char pathname[]="12_server.c"; // The file name used to generate the key.
                             // A file with this name must exist in the current directory.
  key_t  key;     // IPC key
  int i,len, maxlen, error;      // Cycle counter and the length of the informative part of the message

  struct mymsgbuf // Custom structure for the message
  {
    long mtype;
    struct {
      long pid;
      double number;
    } info;
  } mybuf, mybuf2;

  struct msqid_ds msDs;

  if ((key = ftok(pathname,0)) < 0) {
    printf("Can\'t generate key\n");
    exit(-1);
  }

  //
  // Trying to get access by key to the message queue, if it exists,
  // or create it, with read & write access for all users.
  //
  if ((msqid = msgget(key, 0666 | IPC_CREAT | IPC_EXCL )) < 0) {
    printf("Server already exist\n");
    exit(-1);
  } else {
    if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0) {
      printf("Can\'t get queque");
      exit(-1);
    }
  }
  printf("Server started\n");


  /* Send information */

  while (1) {
    len = sizeof(mybuf2.info);

    maxlen = sizeof(mybuf2.info);

    if (msgrcv(msqid, (struct msgbuf *) &mybuf2, maxlen, -2, 0) < 0){
      printf("Can\'t receive message from queue\n");
      msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
      exit(-1);
    }

    if (mybuf2.mtype == 1) {
      break;
    }

    mybuf.mtype = mybuf2.info.pid;
    mybuf.info.number = mybuf2.info.number * mybuf2.info.number;
    mybuf.info.pid = 1;

    len = sizeof(mybuf.info);

    // Send the message. If there is an error,
    // report it and delete the message queue from the system.
    //
    if (msgsnd(msqid, (struct msgbuf *) &mybuf, len, 0) < 0) {
      printf("Can\'t send message to queue\n");
      msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
      exit(-1);
    }

  }

  msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);

  printf("Server stoped\n");

  return 0;
}
