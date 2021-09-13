#pragma once


/** Nodo de cola para bfs */
typedef struct node
{
  /** Nodo seguiente en la cola */
  struct node* next;
  /** Tablero guardado en el nodo */
  int integer;
} Node;

/** Cola para BFS */
typedef struct queue
{
  /** Elemento que fue insertado hace mas tiempo */
  Node* first;
  /** Elemento que fue insertado al ultimo */
  Node* last;
} Queue;

/*Queue* queue_init();

void queue_destroy(Queue* queue);

void queue_print(Queue* queue);
*/
