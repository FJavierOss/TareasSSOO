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
  int id_semaforo = atoi(argv[0]);
  int delay_semaforo = atoi(argv[1]);
  int pid_fabrica = atoi(argv[2]);
  printf("\n > > > > > > > > > > > > > > > > que es estio: %i -- %s", delay_semaforo, argv[1]);
  printf("\n\n\nConectando . . .\n[SEMAFORO] mi PID: %i - mi PADRE: %d - mi DELAY: %s\n\n\n", getpid(), getppid(), argv[0]);
  printf("> > > > ARGUMENTO SEMAFORO:\nID_semaforo: %s \nDELAY: %s\nFABRIA_ID: %s\n\n ", argv[0], argv[1],argv[2]);

//todo: falta enviar la señal correspondiente
  while (1){
    printf("\n el semaforo %i esta enviando señales", delay_semaforo);
    sleep(3);
  }
  

}
