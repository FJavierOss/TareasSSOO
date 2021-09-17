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
  int start_time;
  int* arreglo;
  int size_arreglo;
  int pos_avance_arreglo;
  int quantum;
  int cpu_times; //
  int quantum_times; //
  int turnaround_times; //
  int response_time; //
  int waiting_time; //
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

  int size;

  int elementos;
  int fabrica_1;
  int fabrica_2;
  int fabrica_3;
  int fabrica_4;
} Queue;



Process* process_init(int pid, int n_fabrica, char* nombre, int start_time,
                      int* arreglo, int size_arreglo);

Queue* queue_init();

void queue_insert(Queue* queue, Process* process);

void queue_move_to_last(Queue* queue, int value);

//Process* info_pop(Queue* queue, int pid);

void queue_move_to_first(Queue* queue, int pid);

Process* queue_pop(Queue* queue, int pid);

void queue_destroy(Queue* queue);

void queue_print(Queue* queue);



/* Nodo de cola para estadisticas */
typedef struct node_s
{
  /* Nodo seguiente en la cola */
  struct node_s* next;
  /* Proceso */
  char* info;
} Node_s;

/* Cola */
typedef struct queue_s
{
  /* Elemento que fue insertado hace mas tiempo */
  Node_s* first;
  /* Elemento que fue insertado al ultimo */
  Node_s* last;
} Queue_s;

Queue_s* queue_init_s();

void queue_insert_s(Queue_s* queue, char* info);

void queue_destroy_s(Queue_s* queue);

void queue_print_s(Queue_s* queue);

int get_quantum(Queue* queue, int num_fabrica, int q_entregado);
