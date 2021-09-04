#include <stdio.h>
#include <unistd.h>
#include "../file_manager/manager.h"


/*
void handle_prueba( int sig, siginfo_t *siginfo, void *context)
  {
    printf("Entro a la funcion\n");
    int number = siginfo->si_value.sival_int;
    printf("el numero recibido es: %i\n", number);
    
  }
  */

int main(int argc, char const *argv[])
{

  /*
  printf("\nConectando . . .");
  //connect_sigaction(SIGUSR1,handle_prueba);
  printf("\nHOLI, I'm the SEMAFORO process and my PID is: %i\n ", getpid());
  printf("SEMAFORO - Mi padre es: %d\n", getppid());
  printf("SEMAFORO - Mi delay es: %s\n\n", argv[0]);
  */
  printf("\nConectando . . .\n[SEMAFORO] mi PID: %i - mi PADRE: %d - mi DELAY: %s\n", getpid(), getppid(), argv[0]);

}
