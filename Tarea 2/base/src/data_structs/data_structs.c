#include <stdio.h>  // FILE, fopen, fclose, etc.
#include <string.h> // strtok, strcpy, etc.
#include <stdlib.h>
#include <unistd.h>
#include "data_structs.h"


Process* process_init(int pid, int n_fabrica, char* nombre,
                      int start_time, int* arreglo, int size_arreglo){
  Process* process = calloc(1, sizeof(Process));

  process -> pid = pid;
  process -> n_fabrica = n_fabrica;
  memcpy(process -> name, nombre, strlen(nombre));
  process -> status = 1;
  process -> start_time = start_time;
  process -> pos_avance_arreglo = 0;
  process -> quantum = 0;
  process -> arreglo = arreglo;
  process -> size_arreglo = size_arreglo;

  return process;
}


/* Crea una cola vacia y la devuelve */
Queue* queue_init()
{
  Queue* queue = malloc(sizeof(Queue));
  queue -> first = NULL;
  queue -> last = NULL;
  queue -> elementos = 0;
  queue -> size = 0;

  queue -> fabrica_1 = 0;
  queue -> fabrica_2 = 0;
  queue -> fabrica_3 = 0;
  queue -> fabrica_4 = 0;
  return queue;
}


/** Inserta un elemento al principio de la cola */
void queue_insert(Queue* queue, Process* process)
{
  queue -> size ++;
  // Creo el nodo de la cola a insertar
  Node* node = malloc(sizeof(Node));
  node -> integer = 0;
  node -> process = process;
  node -> next = NULL;

  // Si la cola esta vacia, dejo el nodo como primero y ultimo
  if (queue -> first == NULL)
  {
    queue -> first = node;
    queue -> last = node;
  }
  // Sino, lo dejo como last y conecto el anterior last con el nodo
  else
  {
    queue -> last -> next = node;
    queue -> last = node;
  }
}


// Si un valor dentro del arreglo hace match con el pid requerido
// entonces se toma el nodo que lo contiene y se envia al final del arreglo
void queue_move_to_last(Queue* queue, int pid){
  Node* current = queue -> first;
  Node* parent = current;
  int continua = 1;

  // Aqui se evalua el caso en que el primer elemento de la cola necesita
  // moverse a la ultima posicion
  if(current->process->pid == pid){
    if (current -> next){
      Node* hijo = current -> next;
      queue -> first = hijo;
      queue -> last -> next = current;
      queue -> last = current;
      current -> next = NULL;
    }
    continua = 0;
  }
}


  // Si un valor dentro del arreglo hace match con el pid requerido
// entonces se toma el nodo que lo contiene y se envia al final del arreglo
void queue_move_to_first(Queue* queue, int pid){
  Node* current = queue -> first;
  Node* parent = current;
  int continua = 1;

  // Aqui se evalua el caso en que el primer elemento de la cola no es el que se pide
  if(current->process->pid != pid){
    // Se recorren todos los nodos buscando un valor que haga match
    while ((current -> next)&&(continua)){

      // Aqui si es que un valor hace match, entonces se extrae el nodo
      // y se agrega al final de la cola, esto se hace con redefinicion de punteros
      if(current->process->pid == pid){
        Node* hijo = current -> next;
        parent -> next = hijo;
        current -> next = queue -> first;
        queue -> first = current;
        continua = 0;
      }
      if (continua){
        parent = current;
        Node* next = current -> next;
        current = next;
      }

    }

    if (current->process->pid == pid){
      parent -> next = NULL;
      current -> next = queue -> first;
      queue -> first = current;
    }
  }
}

/** Elimina el nodo con el match de la cola y retorna el proceso correspondiente*/
Process* queue_pop(Queue* queue, int pid)
{

  if (queue -> first){

      Node* current = queue -> first;
      Node* parent = current;
      int continua = 1;

      // Aqui se evalua el caso en que el primer elemento de la cola necesita
      // sacarse
      if(current->process->pid == pid){
        //printf("Entra test 1\n");
        if (current -> next){
          Node* hijo = current -> next;
          queue -> first = hijo;
          current -> next = NULL;
          Process* process = current -> process;
          free(current);
          queue -> size--;
          return process;
        } else {
          queue -> first = NULL;
          queue -> last = NULL;
          Process* process = current -> process;
          free(current);
          queue -> size--;
          return process;
        }
        continua = 0;
      }

      // Se recorren todos los nodos buscando un valor que haga match
      // Este while no considera el ultimo nodo
      while ((current -> next)&&(continua)){

        // Aqui si es que un valor hace match, entonces se extrae el nodo
        // y se agrega al final de la cola, esto se hace con redefinicion de punteros
        if(current->process->pid == pid){
          //printf("Entra test 2\n");
          Node* hijo = current -> next;
          parent -> next = hijo;
          Process* process = current -> process;
          free(current);
          continua = 0;
          queue -> size--;
          return process;
        }
        if (continua){
          //printf("Entra test 3\n");
          parent = current;
          Node* next = current -> next;
          current = next;
        }
      }

      // Se chequea el último nodo
      if (current -> process -> pid == pid){
        //printf("Entra test 4\n");
        queue -> last = parent;
        queue -> last -> next = NULL;
        Process* process = current -> process;
        free(current);
        queue -> size--;
        return process;
      } else {
        // Si es que no se encontro el pid en ningun nodo
        return NULL;
      }

  // Este es el caso si la queue estaba vacia
  } else {
    return NULL;
  }

}


/** Devuelve el p con el match de la cola y retorna el proceso correspondiente*/
Process* info_pop(Queue* queue, int pid)
{

  if (queue -> first){

      Node* current = queue -> first;
      Node* parent = current;
      int continua = 1;
      printf("Test 1 ------");
      printf("%d\n", queue->size);

      // Aqui se evalua el caso en que el primer elemento de la cola necesita
      // sacarse
      if(current->process->pid == pid){
        printf("Test 2 ------");
        //printf("Entra test 1\n");
        Process* process = current -> process;
        return process;
        continua = 0;
      }

      // Se recorren todos los nodos buscando un valor que haga match
      // Este while no considera el ultimo nodo
      while ((current -> next)&&(continua)){

        // Aqui si es que un valor hace match, entonces se extrae el nodo
        // y se agrega al final de la cola, esto se hace con redefinicion de punteros
        if(current->process->pid == pid){
          //printf("Entra test 2\n");          
          Process* process = current -> process;
          return process;
        }
        if (continua){
          //printf("Entra test 3\n");
          parent = current;
          Node* next = current -> next;
          current = next;
        }
      }

      // Se chequea el último nodo
      if (current -> process -> pid == pid){
        //printf("Entra test 4\n");
        Process* process = current -> process;
        return process;
      } else {
        // Si es que no se encontro el pid en ningun nodo
        return NULL;
      }

  // Este es el caso si la queue estaba vacia
  } else {
    return NULL;
  }

}


/** Libera la cola con todos sus nodos */
void queue_destroy(Queue* queue)
{
  // Libera iteretivamente los nodos
  if(queue -> first){




    Node* current = queue -> first;
    while (current)
    {

      Node* next = current -> next;
      free(current->process->arreglo);
      free(current->process);

      free(current);
      current = next;
    }
  }

  // Libera la cola
  free(queue);
}

// Función de prueba para chequear que la cola esta bien implementada
void queue_print(Queue* queue){


  if(queue -> first){
    Node* current = queue -> first;
    printf("\n[");
    while (current -> next){
      printf("%d,", current -> process -> pid);

      Node* next = current -> next;
      current = next;
    }
    printf("%d,", current -> process -> pid);
    printf("]\n");
    printf("Se termino el recorrido\n");
  } else {
    printf("[]\n");
  }


}

// Esta funcion debe retornar el valor quantum
int get_quantum(Queue* queue, int num_fabrica, int q_entregado){
  // esta funcion asume que cuando será llamada se le entregará un n_fabrica que
  // se encuentre en la fila, de lo contrario dará ERROR pq no se puede
  // dividir por 0


  Node* current = queue -> first; //toma el primer nodo


  int n_i = 0;

  // Valores 1 true, 0 false
  int factory_present_1 = 0;
  int factory_present_2 = 0;
  int factory_present_3 = 0;
  int factory_present_4 = 0;

  int continuar = 1;
  // Se recorren todos los nodos
  while (continuar){

    //printf("\nProceso: %d - status: %i\n",current -> process -> pid,current -> process -> status);
    switch (current -> process -> n_fabrica) {

    case 1:
      //printf("Estoy en 1\n");
      factory_present_1 = 1;
      if (1 == num_fabrica){
        n_i++;
      }
      break;

    case 2:
      //printf("Estoy en 2\n");
      factory_present_2 = 1;
      if (2 == num_fabrica){
        n_i++;
      }
      break;

    case 3:
      //printf("Estoy en 3\n");
      factory_present_3 = 1;
      if (3 == num_fabrica){
        n_i++;
      }
      break;

    default:
      //printf("Estoy en 4\n");
      factory_present_4 = 1;
      if (4 == num_fabrica){
        n_i++;
      }
      break;
  }


    // Aqui se pasa al siguiente nodo
    if (current -> next){
      Node* next = current -> next;
      current = next;
    }
    else{
      continuar = 0;
    }

  }

  // estoy fuera del WHILE

  int f = factory_present_1 + factory_present_2 + factory_present_3 + factory_present_4;


  return (q_entregado/(f * n_i)) ;
}

///////////////////////////////////////////////////////////////////////
////////////////////     Cola para Strings      ///////////////////////
///////////////////////////////////////////////////////////////////////

Queue_s* queue_init_s()
{
  Queue_s* queue = calloc(1, sizeof(Queue_s));
  queue -> first = NULL;
  queue -> last = NULL;
  return queue;
}



void queue_insert_s(Queue_s* queue, char* info)
{

  // Creo el nodo de la cola a insertar
  Node_s* node = malloc(sizeof(Node_s));
  node -> info = info;
  node -> next = NULL;

  // Si la cola esta vacia, dejo el nodo como primero y ultimo
  if (queue -> first == NULL)
  {
    queue -> first = node;
    queue -> last = node;
  }
  // Sino, lo dejo como last y conecto el anterior last con el nodo
  else
  {
    queue -> last -> next = node;
    queue -> last = node;
  }
}

void queue_destroy_s(Queue_s* queue)
{
  // Libera iteretivamente los nodos
  Node_s* current = queue -> first;
  while (current)
  {
    Node_s* next = current -> next;
    free(current);
    current = next;
  }

  // Libera la cola
  free(queue);
}

void queue_print_s(Queue_s* queue){

  Node_s* current = queue -> first;
  printf("\n[");
  while (current -> next){
    printf("%s,", current -> info);

    Node_s* next = current -> next;
    current = next;
  }
  printf("%s,", current -> info);
  printf("]\n");
  printf("Se termino el recorrido\n");
}
