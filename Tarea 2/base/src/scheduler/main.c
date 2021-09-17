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
  // Inicializamos la cola de lectura
  Queue* cola_lectura = queue_init();
  Queue* cola_escritura = queue_init();
  Queue* cola = queue_init();
  Process* aux_init;

  InputFile *file = read_file("input.txt");

  int numero_de_procesos = file->len;
  cola -> elementos = numero_de_procesos;

  printf("Reading file of length %i:\n", file->len);
  for (int i = 0; i < file->len; i++)
  {
    char **line = file->lines[i];

    int pid = i;
    char* nombre = line[0];
    int init_time = atoi(line[1]);
    int factory_n = atoi(line[2]);
    int n_bursts = atoi(line[3]);
    int* arreglo = calloc((n_bursts*2)-1, sizeof(int));

    for (int cont = 0; cont < (n_bursts*2)-1; cont ++){
      arreglo[cont] = atoi(line[4+cont]);
    }

    aux_init = process_init(pid, factory_n, nombre, init_time, arreglo, (n_bursts*2)-1);
    queue_insert(cola_lectura, aux_init);
    queue_insert(cola_escritura, aux_init);


    switch (factory_n) {

    case 1:
      cola -> fabrica_1 ++;
      break;

    case 2:
      cola -> fabrica_2 ++;
      break;

    case 3:
      cola -> fabrica_3 ++;
      break;

    default:
      cola -> fabrica_4 ++;
    }


  }
  input_file_destroy(file);

  //Valor por defecto de Q
  int _q = 4;


  // Aqui se deben insertar los nuevos procesos
  // Se iniciará con solo 1 PROCESO
  // Se ingresa el primer proceso a la lista ligada


  int tiempo = 0;
  int cpu_running = 0;
  Node* current;
  Node* next;
  Node* current_select;
  Node* next_select;
  Process* p1;

  // Comienza el proceso
  int buscar_siguiente = 1;
  while ((cola->fabrica_1)||(cola->fabrica_2)||(cola->fabrica_3)||(cola->fabrica_4)){

    // Aqui se revisa si existe un proceso dentro de la cola de lectura que su tiempo inicio
    // sea igual al tiempo actual, y en ese caso lo agrega a la cola principal

    current = cola_lectura -> first;
    //queue_print(cola_lectura);
    for (int contador = 0; contador < cola_lectura -> size; contador++){
      if (tiempo == current -> process -> start_time){
        p1 = queue_pop(cola_lectura, current -> process -> pid);
        queue_insert(cola, p1);
        printf("[t = %d] El proceso %s ha sido creado con pid %d.\n", tiempo, p1 -> name, p1 -> pid);
        break;
      }
      next = current -> next;
      current = next;
    }

    if (cola -> first){

      if (buscar_siguiente){
        current_select = cola -> first;
        if (current_select -> process -> status == RUNNING){
          buscar_siguiente = 0;
        } else {
        for (int contador = 0; contador < cola -> size; contador++){
          if(current_select -> process -> status == READY){
            current_select -> process -> status = RUNNING;
            printf("[t = %d] El proceso %s con pid %d ha pasado de READY a RUNNING.\n",
              tiempo, current_select -> process -> name, current_select -> process -> pid);
            if (current_select -> process -> cpu_times){
              current_select -> process -> cpu_times ++;
            } else {
              current_select -> process -> cpu_times ++;
              current_select -> process -> response_time = tiempo - (current_select -> process -> start_time);
            }
            int n_fabrica_aux = current_select -> process -> n_fabrica;
            current_select -> process -> quantum = get_quantum(cola, n_fabrica_aux, _q);
            queue_move_to_first(cola, current_select -> process -> pid);
            break;
          }
          next_select = current_select -> next;
          current_select = next_select;
        }
        }
      }

      //justo antes de empezar se cambia buscar_siguiente
      buscar_siguiente = 0;
      /*
      printf("\n");
      printf(">PID %d status %d \n",cola->first->process->pid, cola->first->process->status);
      printf(">PID primera posicion %d status %d \n",cola->first->process->pid, cola->first->process->status);
      if(cola->first->next){
        printf(">PID segunda posicion %d status %d \n",cola->first->next->process->pid, cola->first->next->process->status);
      }
      printf("#########TIEMPO %d###########\n", tiempo);
      printf("---\n");
      */

      //resta el tiempo quantum que se le dedica al proceso
      cola -> first -> process -> quantum--;
      //resta el tiempo dedicado a los waiting y al running de la lista completa
      current_select = cola -> first;

      for (int contador = 0; contador < cola -> size; contador++){

        /*
        printf("Cola size %d\n", cola -> size);
        printf("PID %i\n", current_select -> process -> pid);
        printf("FABRICA %d\n", current_select -> process -> n_fabrica);
        printf("Posicion: %i\n", current_select -> process ->pos_avance_arreglo);
        */
        if((current_select -> process -> status == RUNNING)||(current_select -> process -> status == WAITING)){

          if(current_select -> process -> status == WAITING){
            // se descuernta tiempo de waiting
            if(current_select ->process -> arreglo[(current_select -> process -> pos_avance_arreglo)-1] >0){
              current_select ->process -> arreglo[(current_select -> process -> pos_avance_arreglo)-1]--;
            } else {
              // waiting ya espero lo suficiente, pasa a READY
              printf("[t = %d] El proceso %s con pid %d ha pasado de WAITING a READY.\n",
                tiempo, current_select -> process -> name, current_select -> process -> pid);
              buscar_siguiente = 1;
              current_select -> process -> status = READY;
            }

          } else {
            current_select ->process -> arreglo[current_select -> process -> pos_avance_arreglo]--;
          }


          // CAMBIO DE ESTADO
          //largo del arreglo
          int largo_arreglo = (current_select -> process -> size_arreglo)-1;
          /*
          printf("==================\n");
          printf("largo arreglo: %i\n", largo_arreglo);
          printf("posicion en arreglo: %i\n",current_select -> process -> pos_avance_arreglo);
          printf("n procesos: %i\n", current_select -> process -> arreglo[current_select -> process -> pos_avance_arreglo]);
          printf("quantum: %i\n", current_select -> process -> quantum);
          printf("==================\n\n");
          */
          /*
          printf("Pos avance arreglo %d largo arreglo %d \n",current_select -> process -> pos_avance_arreglo, largo_arreglo);
          if (current_select -> process ->status == WAITING){
            printf("arreglo %d \n", current_select -> process -> arreglo[(current_select -> process -> pos_avance_arreglo)-1]);
            printf("Quantum WAITING %i\n", current_select -> process ->quantum);
          }else {
            printf("arreglo %d \n", current_select -> process -> arreglo[current_select -> process -> pos_avance_arreglo]);
            printf("Quantum RUNNING %i\n", current_select -> process ->quantum);
          }
          */


          //running -> Finished
          if (current_select -> process -> pos_avance_arreglo ==  largo_arreglo
          &&  current_select -> process -> arreglo[largo_arreglo] == 0){

            current_select -> process -> status = 3;
            next = current_select -> next;
            p1 = queue_pop(cola, current_select->process->pid);

            printf("[t = %d] El proceso %s con pid %d ha pasado FINISHED.\n",
              tiempo, p1 -> name, p1 -> pid);

            current_select = next;
            //se establece el inicio de busqueda del siguiente proceso que debe
            // ir a cpu
            buscar_siguiente = 1;

            // se resta 1 a los elementos que se deben ejecutar
            cola -> elementos--;

            switch (p1 -> n_fabrica) {
            case 1:
              cola -> fabrica_1--;
              break;

            case 2:
              cola -> fabrica_2--;
              break;

            case 3:
              cola -> fabrica_3--;
              break;

            case 4:
              cola -> fabrica_4--;
              break;

            default:
              printf("Error \n");
              break;
          }

            p1 ->  turnaround_times = tiempo - p1->start_time;
            if(!(p1 -> quantum)){
              p1 -> quantum_times++;
            }
          }


          // running ->  waiting
          else if (current_select -> process -> arreglo[current_select -> process -> pos_avance_arreglo] == 0){
            printf("[t = %d] El proceso %s con pid %d ha pasado de RUNNING a WAITING.\n",
              tiempo, current_select -> process -> name, current_select -> process -> pid);
            current_select -> process -> status = 2;

            if(!(current_select -> process -> quantum)){
              current_select -> process -> quantum_times++;
            }

            //se suma 1 al la posicion de avance del arreglo
            current_select -> process -> pos_avance_arreglo += 2;
            //Mueve este proceso al final de la cola
            next_select = current_select -> next;
            queue_move_to_last(cola,current_select -> process -> pid );
            //printf(">> pid: %i\n", current_select->process->pid);
            current_select = next_select;
            buscar_siguiente = 1;
            //printf(">>cambio variable buscar sgtre %d\n", contador);
            //printf(">>cambio variable buscar sgtre %d\n", buscar_siguiente);


          }

          //running -> Ready
          else if ((current_select -> process -> quantum == 0) && (current_select->process->status == RUNNING)){

            printf("[t = %d] El proceso %s con pid %d ha pasado de RUNNING a READY.\n",
              tiempo, current_select -> process -> name, current_select -> process -> pid);

            current_select -> process -> status = 1;
            current_select -> process -> quantum_times ++;

            //se suma 2 al la posicion de avance del arreglo, asi se salta el espacio de "descanzo"
            //current_select -> process -> pos_avance_arreglo += 2;
            next_select = current_select -> next;
            //Mueve este proceso al final de la cola
            queue_move_to_last(cola,current_select -> process ->pid );
            current_select = next_select;
            //se cambia calor de buscar_siguiente para continuar la busqueda
            buscar_siguiente = 1;


          //Waiting -> Waiting
          }
          else if (current_select->process -> status == WAITING){
            //printf("Sigue en WAITING\n");
            current_select -> process -> waiting_time ++;
            next_select = current_select -> next;
            current_select = next_select;

          //Waiting -> READY
          }
          else if (current_select->process -> status == READY){
            //printf("Sigue en READY\n");
            current_select -> process -> waiting_time ++;
            next_select = current_select -> next;
            current_select = next_select;
          }

          //running -> Running
          else {
            //printf("Sigue en RUNNING\n");
            cpu_running = 1;
            next_select = current_select -> next;
            current_select = next_select;

          }


        } else {
          //printf("Estoy en READY ESPERANDO - bs: %d\n", buscar_siguiente);
          current_select -> process -> waiting_time ++;
          next_select = current_select -> next;
          current_select = next_select;

        }
      }
    }

  if (!cpu_running){
    printf("[t = %d] No hay ningún proceso ejecutando en la CPU\n", tiempo);
  }
  cpu_running = 0;
  tiempo ++;
  }


  printf("Cerrando simulación\n");




  queue_destroy(cola);
  queue_destroy(cola_lectura);

  FILE *file_to_write = fopen("statistics.csv", "w");
  Node* current_escritura;
  Process* p_e;
  current_escritura = cola_escritura -> first;
  while (current_escritura -> next){
    p_e = current_escritura -> process;
    fprintf(file_to_write,"%s, %d, %d, %d, %d, %d\n", p_e->name, p_e->cpu_times,
      p_e->quantum_times, p_e->turnaround_times, p_e->response_time,
      p_e->waiting_time );
    next = current_escritura -> next;
    current_escritura = next;
  }
  p_e = current_escritura -> process;
  fprintf(file_to_write,"%s, %d, %d, %d, %d, %d\n", p_e->name, p_e->cpu_times,
    p_e->quantum_times, p_e->turnaround_times, p_e->response_time,
    p_e->waiting_time );

  fclose(file_to_write);
  queue_destroy(cola_escritura);



}
