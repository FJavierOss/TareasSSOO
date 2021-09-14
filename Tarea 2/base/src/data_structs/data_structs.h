#pragma once

typedef enum process_status{
  RUNNING,
  READY,
  WAITING,
  FINISHED
} ProcessStatus;

typedef struct process{
  int pid;
  int n_fabrica;
  char name[256];
  ProcessStatus status;
} Process;

/* Nodo de cola para bfs */
typedef struct node
{
  /* Nodo seguiente en la cola */
  struct node* next;
  /* Proceso */
  int integer;
  Process* process;

} Node;

/* Cola */
typedef struct queue
{
  /* Elemento que fue insertado hace mas tiempo */
  Node* first;
  /* Elemento que fue insertado al ultimo */
  Node* last;
} Queue;


Process* process_init(int pid, int n_fabrica, char* nombre);

Queue* queue_init();

void queue_insert(Queue* queue, Process* process);

void queue_move_to_last(Queue* queue, int value);

int queue_pop(Queue* queue);

void queue_destroy(Queue* queue);

void queue_print(Queue* queue);
