#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main()
{
  pid_t pid;

  pid  = fork();

  if (pid == -1) {
    printf("Error\n");
  } else if (pid == 0) {
    printf("Child Hello world!\n");
  } else {
    printf("Parent Hello world!\n");
  }

  return 0;
}
