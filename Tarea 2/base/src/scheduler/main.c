#include <stdio.h>  // FILE, fopen, fclose, etc.
#include <string.h> // strtok, strcpy, etc.
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "../data_structs/data_structs.h"
#include "../file_manager/manager.h"

int main(int argc, char **argv)
{
  /*
  printf("Hello T2!\n");

  InputFile *file = read_file("input.txt");

  printf("Reading file of length %i:\n", file->len);
  for (int i = 0; i < file->len; i++)
  {
    char **line = file->lines[i];
    printf(
        "\tProcess %s from factory %s has init time of %s and %s bursts.\n",
        line[0], line[2], line[1], line[3]);
  }
  */


  Queue* cola = queue_init();

  char* s1 = "proceso1";
  Process* p1 = process_init(1001,2, s1);

  char* s2 = "proceso2";
  Process* p2 = process_init(1002,3, s2);

  char* s3 = "proceso3";
  Process* p3 = process_init(1003,2, s3);

  queue_insert(cola, p1);
  queue_insert(cola, p2);
  queue_insert(cola, p3);

  queue_print(cola);


  printf("Proceso: pid %d; nFabrica %d\n", cola -> first -> process -> pid,
  cola -> first -> process  -> n_fabrica);
  printf("Proceso: nombre %s\n", cola -> first -> process -> name);

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

  queue_destroy(cola);

}
