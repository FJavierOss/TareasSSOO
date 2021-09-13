#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include "../file_manager/manager.h"


int index_ = 1;
int n_cambios_semaforo = 0;

void funcion_termino(int sig){
  printf("\n[SEMAFORO] finalizando proceso ->%i\n", getpid());
  printf("\nGuardando documentos . . .\n");

  //cambio el valor
  index_ = 0; // en un futuro me gustaria que solo cambie entre 1 y 0, tal que sea el TRUE y FALSE 
}


void MySignailHandler(int sig){
  printf("\n\n> > > > > >[no hace nada]  FINALIZANDO PROCESO SEMAFORO - PID: %i\n", getpid());


}

int main(int argc, char const *argv[])
{
  int id_semaforo = atoi(argv[0]);
  int delay_semaforo = atoi(argv[1]);
  int pid_fabrica = atoi(argv[2]);
  int estado_semaforo = 1;
  printf("\n > > > > > > > > > > > > > > > > que es estio: %i -- %s", delay_semaforo, argv[1]);
  printf("\n\n\nConectando . . .\n[SEMAFORO] mi PID: %i - mi PADRE: %d - mi DELAY: %s\n\n\n", getpid(), getppid(), argv[0]);
  printf("> > > > ARGUMENTO SEMAFORO:\nID_semaforo: %s \nDELAY: %s\nFABRIA_ID: %s\n\n ", argv[0], argv[1],argv[2]);

//todo: falta enviar la señal correspondiente

  signal(SIGINT, MySignailHandler);
  signal(SIGABRT, funcion_termino);
  
  while (index_){
    
    sleep(delay_semaforo);
    // aqui va el codigo para enviar la señal a fabrica
    // Aquí cambiamos el estado del semaforo (Verde=1, Rojo=0)
    if (estado_semaforo){
      estado_semaforo = 0;
    } else {
      estado_semaforo = 1;
    }

    // Aquí enviamos la señal a la fabrica que indica el identificador del
    // semaforo y en que estado se encuentra
    printf("El semaforo %i esta enviando señales a fabrica %d\n", id_semaforo, pid_fabrica);
    if (id_semaforo == 0) {

      if (estado_semaforo){
        send_signal_with_int(pid_fabrica, 2);
      } else {
        send_signal_with_int(pid_fabrica, 1);
      }
    } else if (id_semaforo == 1){

      if (estado_semaforo){
        send_signal_with_int(pid_fabrica, 4);
      } else {
        send_signal_with_int(pid_fabrica, 3);
      }
    } else if (id_semaforo == 2){

      if (estado_semaforo){
        send_signal_with_int(pid_fabrica, 6);
      } else {
        send_signal_with_int(pid_fabrica, 5);
      }
    }


    n_cambios_semaforo ++;
    //codigo para finalizar el proceso (EMERGENCIA)
    
  }



  //Escribir archivo output
  printf("\n[SEMAFORO] pid:%i a realizar %i\n", getpid(), n_cambios_semaforo);
  
  int length_1 = snprintf(NULL, 0, "%d", n_cambios_semaforo);
  char title[length_1+1];
  sprintf(title, "semaforo_%i.txt", id_semaforo);
  FILE *file_to_write = fopen(title, "w");
  fprintf(file_to_write, "%d", n_cambios_semaforo);
  fclose(file_to_write);

}


