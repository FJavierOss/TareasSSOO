#include <stdio.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
  printf("\nHOLI, I'm the SEMAFORO process and my PID is: %i\n ", getpid());
  printf("SEMAFORO - Mi padre es: %d\n", getppid());
  printf("SEMAFORO - Mi delay es: %s\n\n", argv[0]);
}
