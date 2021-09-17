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
 /*
  char letra[3] = "a";
  char letra_2[3] = "f";
  int algo[3] ={letra,letra_2,"s"} ;
  printf("> %s\n", algo[1]);
  printf("Ya paso lo que queri probar\n");
  sleep(10);
*/
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

    queue_insert(cola_lectura, process_init(pid, factory_n, nombre, init_time, arreglo, (n_bursts*2)-1));


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
  int _q = 2;








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
    printf("=========== # ==========\n");


    // Aqui se revisa si existe un proceso dentro de la cola de lectura que su tiempo inicio
    // sea igual al tiempo actual, y en ese caso lo agrega a la cola principal

    current = cola_lectura -> first;
    queue_print(cola_lectura);
/*
    if (prioridad_de_llegada == 3){
      for (int contador = 0; contador < cola_lectura -> size; contador++){
        if (tiempo == current -> process -> start_time){
          printf("size cola lectura antes %d\n", cola_lectura->size);
          p1 = queue_pop(cola_lectura, current -> process -> pid);
          printf("size cola lectura despues %d\n", cola_lectura->size);

          queue_insert(cola, p1);
          queue_print(cola_lectura);
          printf("###########################################################################################################################################################################################\n\n");
          break;
        }
        next = current -> next;
        current = next;
      }
    }
 */   

    //sleep(1);


    //Se ejecuta el proceso



    if (cola -> first){


      if (buscar_siguiente){
        printf("############################################################\n\n");


        current_select = cola -> first;
        if (current_select -> process -> status == RUNNING){
          buscar_siguiente = 0;
        } else {
        for (int contador = 0; contador < cola -> size; contador++){
          if(current_select -> process -> status == READY){
            current_select -> process -> status = RUNNING;
            printf("[t = %d] El proceso %s con pid %d ha pasado de READY a RUNNING.\n",
              tiempo, current_select -> process -> name, current_select -> process -> pid);
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

      printf("\n");
      printf(">PID %d status %d \n",cola->first->process->pid, cola->first->process->status);
      printf(">PID primera posicion %d status %d \n",cola->first->process->pid, cola->first->process->status);
      if(cola->first->next){
        printf(">PID segunda posicion %d status %d \n",cola->first->next->process->pid, cola->first->next->process->status);

      }
      printf("#########TIEMPO %d###########\n", tiempo);



      printf("---\n");


      //resta el tiempo quantum que se le dedica al proceso
      cola -> first -> process -> quantum--;
      //resta el tiempo dedicado a los waiting y al running de la lista completa
      current_select = cola -> first;

      for (int contador = 0; contador < cola -> size; contador++){

        printf("Cola size %d\n", cola -> size);
        printf("PID %i\n", current_select -> process -> pid);
        printf("FABRICA %d\n", current_select -> process -> n_fabrica);
        printf("Posicion: %i\n", current_select -> process ->pos_avance_arreglo);
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
          printf("Pos avance arreglo %d largo arreglo %d \n",current_select -> process -> pos_avance_arreglo, largo_arreglo);
          if (current_select -> process ->status == WAITING){
            printf("arreglo %d \n", current_select -> process -> arreglo[(current_select -> process -> pos_avance_arreglo)-1]);
            printf("Quantum WAITING %i\n", current_select -> process ->quantum);
          }else {
            printf("arreglo %d \n", current_select -> process -> arreglo[current_select -> process -> pos_avance_arreglo]);
            printf(">Quantum:  %i\n", current_select -> process ->quantum);
          }


          //running -> Finished
          if (current_select -> process -> pos_avance_arreglo ==  largo_arreglo
          &&  current_select -> process -> arreglo[largo_arreglo] == 0){

            //printf("Status cambia a FINISHED\n");
            current_select -> process -> status = 3;
            //elimina el primer elemento de la cola



            
              
           
            next = current_select -> next;
            p1 = queue_pop(cola, current_select->process->pid);
            printf("[t = %d] El proceso %s con pid %d ha pasado FINISHED.\n",
              tiempo, p1 -> name, p1 -> pid);
            current_select = next;
            //se cambia calor de buscar_siguiente para continuar la busqueda
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
          free(p1 -> arreglo);
          free(p1);


          }

          // running ->  waiting
          else if (current_select -> process -> arreglo[current_select -> process -> pos_avance_arreglo] == 0){
            printf("[t = %d] El proceso %s con pid %d ha pasado de RUNNING a WAITING.\n",
              tiempo, current_select -> process -> name, current_select -> process -> pid);
            current_select -> process -> status = 2;


            //se suma 1 al la posicion de avance del arreglo
            current_select -> process -> pos_avance_arreglo += 2;
        
            //Mueve este proceso al final de la cola
            next_select = current_select -> next;
            queue_move_to_last(cola,current_select -> process -> pid );
            //printf(">> pid: %i\n", current_select->process->pid);
            current_select = next_select;
            buscar_siguiente = 1;
            printf(">>cambio variable buscar sgtre %d\n", contador);
            printf(">>cambio variable buscar sgtre %d\n", buscar_siguiente);
            



          }

          //running -> Ready
          else if ((current_select -> process -> quantum == 0) && (current_select->process->status == RUNNING)){
            
            
            printf("[t = %d] El proceso %s con pid %d ha pasado de RUNNING a READY.\n",
              tiempo, current_select -> process -> name, current_select -> process -> pid);
            current_select -> process -> status = 1;
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
            printf("Sigue en WAITING\n");
            next_select = current_select -> next;
            current_select = next_select;

          //READY -> READY
          }
          else if (current_select->process -> status == READY){
            printf("READY ---> READY\n");
            printf(">QUANTUM: %i de PID: %i\n", current_select ->process -> quantum, current_select ->process ->pid);
            next_select = current_select -> next;
            current_select = next_select;
          }

          //running -> Running
          else {
            printf("RUINNING ---> RUNNING\n");
            printf(">QUANTUM: %i de PID: %i\n", current_select ->process -> quantum, current_select ->process ->pid);
            printf(">CPU: %i de PID: %i\n", current_select ->process -> arreglo[current_select ->process -> pos_avance_arreglo], current_select ->process ->pid);

            cpu_running = 1;
            next_select = current_select -> next;
            current_select = next_select;

          }


        } else {


          printf("Estoy en READY ESPERANDO - bs: %d\n", buscar_siguiente);
          next_select = current_select -> next;
          current_select = next_select;

        }


      }


  }


  
  int lista_pid[8];
  int lista_n_fabrica[8];
  //const char *lista_name[8];
  int desempate = 0;
  int pid_para_agregar;
  for (int contador = 0; contador < cola_lectura -> size; contador++){
    if (tiempo >= current -> process -> start_time){    // TIEMPO debe ser menor o igual
      lista_pid[contador] = current->process->pid;
      lista_n_fabrica[contador] = current->process->n_fabrica;

      //lista_name[contador] = current->process->name;

      pid_para_agregar = current->process->pid;
      desempate++;
    }
    next = current -> next;
    current = next;
  }

  // si no hay desempate seguimos el proceso habitual
  if (desempate == 1 ){
    printf("size cola lectura antes %d\n", cola_lectura->size);
    p1 = queue_pop(cola_lectura, pid_para_agregar);
    printf("size cola lectura despues %d\n", cola_lectura->size);
    printf("[t = %d] El proceso %s ha sido creado con pid %d.\n", tiempo, p1 -> name, p1 -> pid);
    queue_insert(cola, p1);
    
    queue_print(cola_lectura);
    printf("###########################################################################################################################################################################################\n\n");
    //break;
  }
  else{
    // SI entro aqui quiere decir que hay más de un proceso que quiere entrar

    //Desempate por menor numero de fabrica
    int que_fabrica = 5; // se asume un maximo de 4 fabricas
    int desempate_2 = 0;
    
    int ultima_list_desesmpate[8];
    int count_2 = 0;

    for (int index = 0; index < desempate; index++){
      int aux = lista_n_fabrica[index];
      if (aux < que_fabrica){
        que_fabrica = aux;
        pid_para_agregar = lista_pid[index];
        ultima_list_desesmpate[index] = lista_pid[index];
        count_2++;
        desempate_2++;
      }
    }

    //si hay 1 con menor numeor de fabrica      
    if (desempate_2 == 1){
      printf("size cola lectura antes %d\n", cola_lectura->size);
      p1 = queue_pop(cola_lectura, pid_para_agregar);
      printf("size cola lectura despues %d\n", cola_lectura->size);
      printf("[t = %d] El proceso %s ha sido creado con pid %d.\n", tiempo, p1 -> name, p1 -> pid);
      queue_insert(cola, p1);
      
      queue_print(cola_lectura);
      printf("###########################################################################################################################################################################################\n\n");
      //break;
    }

    /*else{
      // si estamo aca es porque aun no hay desempate
      //char nombre_mas_corto[256];
      int var_aux;
      Process* pro_1 = info_pop(cola_lectura,ultima_list_desesmpate[0]);
      printf("---%d", pro_1 -> pid);
      
      

      for (int indice = 0; indice < desempate_2 - 1 ; indice++){
        Process* pro_2 = info_pop(cola_lectura, ultima_list_desesmpate[indice + 1]);        
        int aux_1 = strcmp(pro_1->name,pro_2 -> name);
        if (aux_1 > 0){
          // si el resultado es +, pro_1 es menor
          var_aux = pro_1->pid;

        }   
        else{
          var_aux = pro_2->pid;
        }      
      }
        //ya tengo el nombre mas corto
      for (int num = 0 ; num < desempate_2 ; num++){
        if (   1){
          // ahora se procede a agregarlo a la cola
          printf("size cola lectura antes %d\n", cola_lectura->size);
          p1 = queue_pop(cola_lectura, lista_pid[num]);
          printf("size cola lectura despues %d\n", cola_lectura->size);

          queue_insert(cola, p1);
          queue_print(cola_lectura);
          printf("###########################################################################################################################################################################################\n\n");
          break;


          }
        }
    } */
  }
    
  

  // justo antes de que se reinicie el ciclo se retorna el valor original  
  // tiempo de prioridad
  if (!cpu_running){
    printf("[t = %d] No hay ningún proceso ejecutando en la CPU\n", tiempo);
  }
  cpu_running = 0;
  tiempo ++;
  }





  printf("Salimos del wahile\n");




  queue_destroy(cola);
  queue_destroy(cola_lectura);


}
