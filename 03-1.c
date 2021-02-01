#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main()
{
  pid_t pid, ppid;

  pid  = getpid();
  ppid = getppid();

  printf("My pid = %d, my ppid = %d\n", (int)pid, (int)ppid);

  // При многократном запуске ppid не менялся, так как мы получаем его
  // от родителя, а pid каждый раз выдается новый.

  return 0;
}
