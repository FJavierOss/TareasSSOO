#include <stdio.h>  // FILE, fopen, fclose, etc.
#include <string.h> // strtok, strcpy, etc.
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "../data_structs/data_structs.h"
#include "../file_manager/manager.h"

int main(int argc, char **argv)
{
  printf("Hello T2!\n");

  InputFile *file = read_file("input.txt");

  printf("Reading file of length %i:\n", file->len);
  int dio = 1;
  for (int i = 0; i < file->len; i++)
  {
    char **line = file->lines[i];


    char* nombre = line[0];
    printf(
        "\tProcess %s .\n", nombre);
    int intit_time = atoi(line[1]);
    printf("\t has init time of %d \n",intit_time);
    int factory_n = atoi(line[2]);

    printf("\t from factory %d \n",factory_n);
    int n_bursts = atoi(line[3]);
    printf("\t and %d bursts.\n", n_bursts);



    int bursts[(n_bursts*2)-1];
    printf("burst size %ld\n", sizeof(bursts));

    printf("[");
    for (int cont = 0; cont < sizeof(bursts)/4; cont ++){
      bursts[cont] = atoi(line[4+cont]);
      printf("%d,", bursts[cont]);
    }
    printf("]\n");

    //printf("Size bursts %ld n_bursts %d\n", sizeof(bursts)/4, n_bursts);
    printf(
        "\tProcess %s from factory %d has init time of %d and %d bursts.\n",
        nombre, factory_n, intit_time, n_bursts);

  }
  input_file_destroy(file);
  
  //Valor por defecto de Q
  int _q = 100;

  Queue* cola = queue_init();

  char* s1 = "proceso1";
  
  int arreglo[9] = {1,2,3,4,1,1,1,1,2};
  int arreglo_2[9] = {1,2,3,4,2,1,1,1,3};
 
  Process* p1 = process_init(1001,2, s1, 5, arreglo);
  
  char* s2 = "proceso2";
  Process* p2 = process_init(1002,3, s2,7, arreglo_2);

  char* s3 = "proceso3";
  Process* p3 = process_init(1003,2, s3, 8, arreglo);

  
  
  // Aqui se deben insertar los nuevos procesos

  
  /*
  queue_insert(cola, p1);
  queue_insert(cola, p2);
  queue_insert(cola, p3);
  
  queue_print(cola);
  */
  
  // Se iniciará con solo 1 PROCESO

  // Se ingresa el primer proceso a la lista ligada

  queue_insert(cola, p1);
  queue_insert(cola, p2);
  printf(">> pid: %i\n", cola -> first->process->pid);
  
  switch (cola -> first -> process -> status) {
    case RUNNING:
      printf("En running\n");
      break;

    case READY:
      printf("En ready\n");
      break;

    case WAITING:
      printf("En waiting\n");
      break;

    case FINISHED:
      printf("En finished\n");
      break;

    default:  
      printf("Nothing\n");
      break;
  } 



  // Comienza el proceso
  
  while (cola -> first){
    

    printf("\n");
    printf(">PID: %i\n", cola->first->process->pid);
    // Se elige a un proceso para running
    cola -> first -> process -> status = 1;


    int n_fabrica_aux = cola -> first -> process -> n_fabrica;
    cola -> first -> process -> quantum = get_quantum(cola, n_fabrica_aux, _q);

    //Se ejecuta el proceso
    //int index = cola -> first -> process -> pos_avance_arreglo;

    
    while (1){
    
      //printf(">>>\n");

      cola -> first -> process -> arreglo[cola -> first -> process -> pos_avance_arreglo]--;
      cola -> first -> process -> quantum--;  
  
    
    
      // CAMBIO DE ESTADO

      //largo del arreglo
      int largo_arreglo = sizeof(cola -> first -> process -> arreglo)/sizeof(cola -> first -> process -> arreglo[0])-1;
      /*
      printf("==================\n");
      printf("largo arreglo: %i\n", largo_arreglo);
      printf("posicion en arreglo: %i\n",cola -> first -> process -> pos_avance_arreglo);
      printf("n procesos: %i\n", cola -> first -> process -> arreglo[cola -> first -> process -> pos_avance_arreglo]);
      printf("quantum: %i\n", cola -> first -> process -> quantum);
      printf("==================\n\n");
      */
      //Finished
      if (cola -> first -> process -> pos_avance_arreglo ==  largo_arreglo 
      &&  cola -> first -> process -> arreglo[largo_arreglo] == 0){

        //printf("Status cambia a FINISHED\n");
        cola -> first -> process -> status = 3;
        //elimina el primer elemento de la cola
        printf("[FINALIZADO]>> pid: %i\n", cola -> first->process->pid);
        queue_pop(cola);
        
        break;
      }  

      //Waiting
      else if (cola -> first -> process -> arreglo[cola -> first -> process -> pos_avance_arreglo] == 0){
        printf("Status cambia a Waiting\n");
        cola -> first -> process -> status = 2;

        //se suma 1 al la posicion de avance del arreglo
        cola -> first -> process -> pos_avance_arreglo++;
        //Mueve este proceso al final de la cola
        queue_move_to_last(cola,cola -> first -> process ->pid );
        //printf(">> pid: %i\n", cola -> first->process->pid);
        break;
      }

      //Ready
      else if (cola -> first -> process -> quantum == 0){
        printf("Status cambia a Ready\n");
        cola -> first -> process -> status = 1;

        //se suma 1 al la posicion de avance del arreglo
        cola -> first -> process -> pos_avance_arreglo++;
        //Mueve este proceso al final de la cola
        queue_move_to_last(cola,cola -> first -> process ->pid );
        //printf(">> pid: %i\n", cola -> first->process->pid);
        break;
      }

      //Running
      else {
        printf("Sigue en RUNNING\n");
        
      }

  }


  }





/*
  printf("Proceso: pid %d; nFabrica %d\n", cola -> first -> process -> pid,
  cola -> first -> process  -> n_fabrica);
  printf("Proceso: nombre %s\n", cola -> first -> process -> name);
  printf("Proceso: STATUS %i\n", cola -> first -> process -> status);

  switch (cola -> first -> process -> status) {
    case RUNNING:
      printf("En running\n");
      break;

    case READY:
      printf("En ready\n");
      break;

    case WAITING:
      printf("En waiting\n");
      break;

    case FINISHED:
      printf("En finished\n");
      break;

    default:  
      printf("Nothing\n");
      break;
  }
*/
  queue_destroy(cola);

}


/*
if (cola -> first -> process -> status == 2){
      printf("Paso a waiting\n");
    }
    else if (cola -> first -> process -> status == 3){
      printf("Paso a finished\n");
    }

    else if (cola -> first -> process -> status == 1){
      printf("Paso a ready\n");
    }
    else (cola -> first -> process -> status == 0){
      printf("Continuo en running\n");
    }
*/