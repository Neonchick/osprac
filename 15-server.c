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
int client_pid;
int can_send = 0;
int can_start = 0;

void client_ans(int nsig) {
  can_send = 1;
}

void client_exist(int nsig) {
  can_start = 1;
}

int main(void) {
  (void)signal(SIGUSR1, client_ans);
  (void)signal(SIGUSR2, client_exist); 
  printf("Server pid: %d\n", getpid());
  printf("Write client pid\n");
  scanf("%d", &client_pid);
  printf("Write number\n");
  scanf("%d", &number);
  printf("Waiting client\n");
  while(!can_start);
  while(currentBit!=0) {
    if (currentBit & number) {
      kill(client_pid, SIGUSR1);
    } else {
      kill(client_pid, SIGUSR2);
    }
    currentBit = (currentBit >> 1) & andBit;
    while(!can_send);
    can_send = 0;
  }
  printf("Nuber sent\n");
  return 0;
}
