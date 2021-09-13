#include <stdio.h>  // FILE, fopen, fclose, etc.
#include <string.h> // strtok, strcpy, etc.
#include <stdlib.h>
#include "data_structs.h"


/** Crea una cola vacia */
Queue* queue_init()
{
  // Creo la cola
  Queue* queue = malloc(sizeof(Queue));

  // Dejo los valores en NULL
  queue -> first = NULL;
  queue -> last = NULL;

  // Retorno la cola
  return queue;
}


/** Inserta un elemento al principio de la cola */
void queue_insert(Queue* queue, int integer)
{

  // Creo el nodo de la cola a insertar
  Node* node = malloc(sizeof(Node));
  node -> integer = integer;
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

/** Elimina el primer nodo de la cola y lo retorna */
int queue_pop(Queue* queue)
{
  // Nodo a eliminar
  Node* poped = queue -> first;

  // Si no quedan elementos retorno NULL
  if (!poped) return NULL;

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
    printf("%d,", current -> integer);

    Node* next = current -> next;
    current = next;
  }
  printf("%d,", current -> integer);
  printf("]\n");
  printf("Se termino el recorrido\n");
}
