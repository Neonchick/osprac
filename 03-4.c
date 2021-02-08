#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[], char *envp[])
{
  (void) execle("./03-3.out", "./03-3.out", 0, envp);
  //
  // We get here only when an error occurs.
  //
  printf("Error on program start\n");
  exit(-1);
}

