#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include "../file_manager/manager.h"

void noDetenerProceso(int sig){
  printf("\n\n . . . . . . . Pasa por repartidor pero no hace nada\n");
}

int main(int argc, char const *argv[])
{
  // Guardamos las ubicaciones para poder utilizarlas cuando se requiera llamar
  // a la fabrica para preguntar por los semaforos, se hizo esto para evitar
  // llamar constantemente a la fabrica en cada turno y no colapsar el proceso
  int ubicacion_s1 = atoi(argv[1]);
  int ubicacion_s2 = atoi(argv[2]);
  int ubicacion_s3 = atoi(argv[3]);
  int ubicacion_b = atoi(argv[4]);
  printf("\n\nI'm the REPARTIDOR process and my PID is: %i\n", getpid());
  printf("Distancias: S1-%d ; S2-%d ; S3-%d ; B-%d\n",ubicacion_s1, ubicacion_s2, ubicacion_s3, ubicacion_b);

  int repartidor_id = argv[0][0];

  printf("\n[REPARTIDOR] id %d listo para trabajar\n", repartidor_id);

  int pasos = 0;

  int turnos_s1 = 0;
  int turnos_s2 = 0;
  int turnos_s3 = 0;
  int turnos_b = 0;


  // señal SIGINT- DEBE IGNORAR
  signal(SIGINT, noDetenerProceso);
  
  // Aqui empezamos con los turnos del repartidor
  for (int i = 0; i < ubicacion_b; i++){
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

    pasos += 1;
    printf("Repartidor id %d avanzando a distancia %d\n",repartidor_id, pasos);
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

  char title[16 + digitos_en_numero];
  sprintf(title, "repartidor_%d.txt", repartidor_id);
  FILE *file_to_write = fopen(title, "w");
  fprintf(file_to_write, "%d,%d,%d,%d", turnos_s1, turnos_s2, turnos_s3, turnos_b);
  fclose(file_to_write);

  send_signal_with_int(getppid(), 1000);
}