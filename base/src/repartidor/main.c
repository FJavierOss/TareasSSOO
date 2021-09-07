#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include "../file_manager/manager.h"


void MySignailHandler_2(int sig){
 printf("\n[REPARTIDOR] No hace nada\n");
}



int main(int argc, char const *argv[])
{

  // Definimos los estados de los semaforos
  // Si el semaforo esta en 1 significa que esta en verde y rojo con 0
  int repartidor_id = argv[0][0];
  int estado_s1 = atoi(argv[5]);
  int estado_s2 = atoi(argv[6]);
  int estado_s3 = atoi(argv[7]);


  // Guardamos las ubicaciones para poder utilizarlas cuando se requiera llamar
  // a la fabrica para preguntar por los semaforos, se hizo esto para evitar
  // llamar constantemente a la fabrica en cada turno y no colapsar el proceso

  int ubicacion_s1 = atoi(argv[1]);
  int ubicacion_s2 = atoi(argv[2]);
  int ubicacion_s3 = atoi(argv[3]);
  int ubicacion_b = atoi(argv[4]);

  printf("\nI'm the REPARTIDOR process and my PID is: %i\n", getpid());
  printf("Distancias: S1-%d ; S2-%d ; S3-%d ; B-%d\n",
  ubicacion_s1, ubicacion_s2, ubicacion_s3, ubicacion_b);


  // Definimos el resto de las variables para la ejecución del proceso

  int pasos = 0;
  int turnos_s1 = 0;
  int turnos_s2 = 0;
  int turnos_s3 = 0;
  int turnos_b = 0;
  printf("\nRepartidor id %d listo para trabajar\n\n", repartidor_id);
  int continuar = 1;

  void finalizar_repartidor(){
    printf("\n[REPARTIDOR] finalizando");
    continuar = 0;
  }
  //sEÑAL DE TERMINO
  signal(SIGINT, MySignailHandler_2); 
  signal(SIGABRT, finalizar_repartidor);


  // Definimos la función que actualiza los semaforos segun las señales recibidas
  void handle_repartidor(int test_info, siginfo_t *siginfo, void *context){

    if (siginfo->si_value.sival_int == 1){
      estado_s1 = 0;
      printf("\nCambio de estado semaforo 1 de 1 a 0\n");
    } else if (siginfo->si_value.sival_int == 2){
      estado_s1 = 1;
      printf("\nCambio de estado semaforo 1 de 0 a 1\n");
    } else if (siginfo->si_value.sival_int == 3){
      estado_s2 = 0;
      printf("\nCambio de estado semaforo 2 de 1 a 0\n");
    } else if (siginfo->si_value.sival_int == 4){
      estado_s2 = 1;
      printf("\nCambio de estado semaforo 2 de 0 a 1\n");
    } else if (siginfo->si_value.sival_int == 5){
      estado_s3 = 0;
      printf("\nCambio de estado semaforo 3 de 1 a 0\n");
    } else if (siginfo->si_value.sival_int == 6){
      estado_s3 = 1;
      printf("\nCambio de estado semaforo 3 de 0 a 1\n");
    }
    printf("Repartidor id %d Semaforo update s %d s %d s %d\n",repartidor_id,
    estado_s1, estado_s2, estado_s3);
    printf("Recibiendo señal del semaforo %d\n", siginfo->si_value.sival_int);
  }

  connect_sigaction(SIGUSR1, &handle_repartidor);


/* COPIE ESTO MAS ARRIBA [ORIGINAL]
  // Guardamos las ubicaciones para poder utilizarlas cuando se requiera llamar
  // a la fabrica para preguntar por los semaforos, se hizo esto para evitar
  // llamar constantemente a la fabrica en cada turno y no colapsar el proceso

  int ubicacion_s1 = atoi(argv[1]);
  int ubicacion_s2 = atoi(argv[2]);
  int ubicacion_s3 = atoi(argv[3]);
  int ubicacion_b = atoi(argv[4]);

  printf("\nI'm the REPARTIDOR process and my PID is: %i\n", getpid());
  printf("Distancias: S1-%d ; S2-%d ; S3-%d ; B-%d\n",
  ubicacion_s1, ubicacion_s2, ubicacion_s3, ubicacion_b);


  // Definimos el resto de las variables para la ejecución del proceso

  int pasos = 0;
  int turnos_s1 = 0;
  int turnos_s2 = 0;
  int turnos_s3 = 0;
  int turnos_b = 0;
  printf("\nRepartidor id %d listo para trabajar\n\n", repartidor_id);
*/

  // Aqui empezamos con los turnos del repartidor

  while (pasos < ubicacion_b && continuar){
    if (pasos < ubicacion_s1){
      turnos_s1 += 1;
    }

    if (pasos < ubicacion_s2){
      turnos_s2 += 1;
    }

    if (pasos < ubicacion_s3){
      turnos_s3 += 1;
    }

    if (pasos < ubicacion_b){
      turnos_b += 1;
    }

    printf("Repartidor id %d pasando el s %d s %d s %d\n",repartidor_id,
    estado_s1, estado_s2, estado_s3);
    if ( pasos + 1 == ubicacion_s1 ){
      if ( estado_s1){
        pasos += 1;
        printf("Repartidor id %d pasando el semaforo 1\n",repartidor_id);
      } else {
        printf("------------------Repartidor id %d esperando a semaforo 1\n",repartidor_id);
      }

    } else if ( pasos + 1 == ubicacion_s2 ){
      if ( estado_s2){
        pasos += 1;
        printf("Repartidor id %d pasando el semaforo 2\n",repartidor_id);
      } else {
        printf("------------------Repartidor id %d esperando a semaforo 2\n",repartidor_id);
      }

    } else if ( pasos + 1 == ubicacion_s3 ){
      if ( estado_s3){
        pasos += 1;
        printf("Repartidor id %d pasando el semaforo 3\n",repartidor_id);
      } else {
        printf("------------------Repartidor id %d esperando a semaforo 3\n",repartidor_id);
      }
    } else {
      pasos += 1;
      printf("Repartidor id %d avanzando %d\n",repartidor_id, pasos);
    }

    sleep(1);
  }

  // Esto es para para obtener las cifras de un numero para poner bien el nombre
  // del archivo
  int digitos_en_numero = 0;
  int n = repartidor_id;
  do {
    n /= 10;
    ++digitos_en_numero;
  } while (n != 0);


  if (pasos < ubicacion_s1){
    turnos_s1 = -1;
  }

  if (pasos < ubicacion_s2){
    turnos_s2 = -1;
  }

  if (pasos < ubicacion_s3){
    turnos_s3 = -1;
  }
  
  if (pasos < ubicacion_b){
    turnos_b = -1;
  }
  
  char title[16 + digitos_en_numero];
  sprintf(title, "repartidor_%d.txt", repartidor_id);
  FILE *file_to_write = fopen(title, "w");
  fprintf(file_to_write, "%d,%d,%d,%d", turnos_s1, turnos_s2, turnos_s3, turnos_b);
  fclose(file_to_write);

}