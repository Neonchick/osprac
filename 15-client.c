#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <wait.h>
#include <errno.h>
#include <stdlib.h>

int currentBit = 0X80000000;
int andBit = 0X7FFFFFFF;
int number = 0;
int server_pid;
int server_exist = 0;

void add_one(int nsig) {
  server_exist = 1;
  number |= currentBit;
  currentBit = (currentBit >> 1) & andBit;
  kill(server_pid, SIGUSR1);
}

void add_zero(int nsig) {
  server_exist = 1;
  currentBit = (currentBit >> 1) & andBit;
  kill(server_pid, SIGUSR1);
}

int main(void) {
  (void)signal(SIGUSR1, add_one);
  (void)signal(SIGUSR2, add_zero);
  printf("Client pid: %d\n", getpid());
  printf("Write server pid\n");
  scanf("%d", &server_pid);
  printf("Waiting number from server\n");
  while(!server_exist) {
    kill(server_pid, SIGUSR2);
  }
  while(currentBit!=0);
  printf("%d\n", number);
  return 0;
}
