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
  Queue* cola = queue_init();

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

    queue_insert(cola_lectura, process_init(pid, factory_n, nombre, init_time, arreglo, n_bursts));

    
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
  int _q = 20;


  



  
  
  // Aqui se deben insertar los nuevos procesos

  
  
  // Se iniciará con solo 1 PROCESO

  // Se ingresa el primer proceso a la lista ligada
 
 
  int tiempo = 0;
  Node* current;
  Node* next;
  Node* current_select;
  Node* next_select;

  // Comienza el proceso
  
  while ((cola->fabrica_1)||(cola->fabrica_2)||(cola->fabrica_3)||(cola->fabrica_4)){
    printf("=========== # ==========\n");
    

    // Aqui se revisa si existe un proceso dentro de la cola de lectura que su tiempo inicio
    // sea igual al tiempo actual, y en ese caso lo agrega a la cola principal
    
    current = cola_lectura -> first;
    for (int contador = 0; contador < cola_lectura -> size; contador++){
      if (tiempo == current -> process -> start_time){
        Process* p1 = queue_pop(cola_lectura, current -> process -> pid);
        queue_insert(cola, p1);        
        break;
      }
      next = current -> next;
      current = next; 
    }
    
    sleep(1);
    

    //Se ejecuta el proceso


    
    if (cola -> first){
      printf("\n");
      printf(">PID: %i\n", cola->first->process->pid);
      //printf(">>>\n");

      current_select = cola -> first;
      for (int contador = 0; contador < cola -> size; contador++){
        if(current_select -> process -> status == READY){
          current_select -> process -> status = RUNNING;
          int n_fabrica_aux = current_select -> process -> n_fabrica;
          current_select -> process -> quantum = get_quantum(cola, n_fabrica_aux, _q);
          queue_move_to_first(cola, current_select -> process -> pid);
          break;
        }

        next_select = current_select -> next;
        current_select = next_select;
      }

      printf("PID %d status %d \n",cola->first->process->pid, cola->first->process->status);
      if (cola->first->next){
        printf("PID %d status %d \n",cola->last->process->pid, cola->last->process->status);  
      }
      
      printf("---\n");
      

      //resta el tiempo quantum que se le dedica al proceso
      cola -> first -> process -> quantum--;  
      //resta el tiempo dedicado a los waiting y al running de la lista completa
      current_select = cola -> first;
      for (int contador = 0; contador < cola -> size; contador++){
        if((current_select -> process -> status == RUNNING)||(current_select -> process -> status == WAITING)){

          
          current_select ->process -> arreglo[current_select -> process -> pos_avance_arreglo]--;

          // CAMBIO DE ESTADO
          //largo del arreglo
          int largo_arreglo = (current_select -> process -> size_arreglo) - 1;
          
          printf("==================\n");
          printf("largo arreglo: %i\n", largo_arreglo);
          printf("posicion en arreglo: %i\n",current_select -> process -> pos_avance_arreglo);
          printf("n procesos: %i\n", current_select -> process -> arreglo[current_select -> process -> pos_avance_arreglo]);
          printf("quantum: %i\n", current_select -> process -> quantum);
          printf("==================\n\n");
          
          //Finished
          if (current_select -> process -> pos_avance_arreglo ==  largo_arreglo 
          &&  current_select -> process -> arreglo[largo_arreglo] == 0){

            //printf("Status cambia a FINISHED\n");
            current_select -> process -> status = 3;
            //elimina el primer elemento de la cola
            printf("[FINALIZADO]>> pid: %i\n", current_select->process->pid);
            queue_pop(cola, current_select->process->pid);
            
            // se resta 1 a los elementos que se deben ejecutar
            cola -> elementos--;

            switch (current_select -> process -> n_fabrica) {
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
            
            
          }  

          //Waiting
          else if (current_select -> process -> arreglo[current_select -> process -> pos_avance_arreglo] == 0){
            printf("Status cambia a Waiting\n");
            current_select -> process -> status = 2;
            

            //se suma 1 al la posicion de avance del arreglo
            current_select -> process -> pos_avance_arreglo += 2;
            //Mueve este proceso al final de la cola
            queue_move_to_last(cola,current_select -> process -> pid );
            //printf(">> pid: %i\n", current_select->process->pid);
            
          }

          //Ready
          else if (current_select -> process -> quantum == 0){
            printf("Status cambia a Ready\n");
            current_select -> process -> status = 1;

            //se suma 2 al la posicion de avance del arreglo, asi se salta el espacio de "descanzo"
            current_select -> process -> pos_avance_arreglo += 2;
            //Mueve este proceso al final de la cola
            queue_move_to_last(cola,current_select -> process ->pid );
            
          }

          //Running
          else {
            printf("Sigue en RUNNING\n");
            
          }


        }

        next_select = current_select -> next;
        current_select = next_select;
      }
     

  }

  tiempo ++;
  }





  printf("Salimos del wahile\n");
  queue_destroy(cola);
  queue_destroy(cola_lectura);

}

