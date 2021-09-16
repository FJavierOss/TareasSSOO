#include <stdio.h>  // FILE, fopen, fclose, etc.
#include <string.h> // strtok, strcpy, etc.
#include <stdlib.h>
#include <unistd.h>
#include "data_structs.h"


Process* process_init(int pid, int n_fabrica, char* nombre, int start_time, int arreglo[]){
  Process* process = calloc(1, sizeof(Process));

  process -> pid = pid;
  process -> n_fabrica = n_fabrica;
  memcpy(process -> name, nombre, strlen(nombre));
  process -> status = 1;
  process -> start_time = start_time;

  memcpy(process -> arreglo, arreglo, 10*4);
  printf("@@@@@@\n");
  for (int a=0; a<9;a++){
    printf("%i\n", process ->arreglo[a]);
  }
  process -> pos_avance_arreglo = 0;
  process -> quantum;

  return process;
}


/* Crea una cola vacia y la devuelve */
Queue* queue_init()
{
  Queue* queue = malloc(sizeof(Queue));
  queue -> first = NULL;
  queue -> last = NULL;
  return queue;
}


/** Inserta un elemento al principio de la cola */
void queue_insert(Queue* queue, Process* process)
{

  // Creo el nodo de la cola a insertar
  Node* node = malloc(sizeof(Node));
  node -> integer = NULL;
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

  // Se recorren todos los nodos buscando un valor que haga match
  while ((current -> next)&&(continua)){

    // Aqui si es que un valor hace match, entonces se extrae el nodo
    // y se agrega al final de la cola, esto se hace con redefinicion de punteros
    if(current->process->pid == pid){
      Node* hijo = current -> next;
      parent -> next = hijo;
      queue -> last -> next = current;
      queue -> last = current;
      current -> next = NULL;
      continua = 0;
    }
    if (continua){
      parent = current;
      Node* next = current -> next;
      current = next;
    }

  }
}

/** Elimina el primer nodo de la cola y lo retorna */
int queue_pop(Queue* queue)
{

  Node* poped = queue -> first;

  // Si no quedan elementos retorno NULL
  if (!poped) return 0;

  // Sino, dejo el siguiente como first
  queue -> first = poped -> next;

  // Si no quedan nodos entonces last es NULL
  if (!queue -> first) queue -> last = NULL;

  // Obtengo el integer y libero el nodo
  int integer = poped -> integer;
  free(poped);

  // Retorno el intger
  return integer;
}

/** Libera la cola con todos sus nodos */
void queue_destroy(Queue* queue)
{
  // Libera iteretivamente los nodos
  Node* current = queue -> first;
  while (current)
  {
    Node* next = current -> next;
    free(current->process);
    free(current);
    current = next;
  }

  // Libera la cola
  free(queue);
}

// Función de prueba para chequear que la cola esta bien implementada
void queue_print(Queue* queue){

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