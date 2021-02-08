#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char *argv[], char *envp[])
{
  printf("Command line parameters:\n");
  for (int i=0;i<argc;i++) {
    printf(argv[i]);
    printf("\n");
  }
  printf("\nEnvironmental parameters:\n");
  int i = 0;
  while (envp[i]) {
    printf(envp[i]);
    printf("\n");
    i++;
  }
  printf("\n");
  return 0;
}
