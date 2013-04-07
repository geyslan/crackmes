#include <stdio.h>


long ptrace(int request, int pid, int addr, int data)
{
  printf("I'm fake");
  return 0;
}
