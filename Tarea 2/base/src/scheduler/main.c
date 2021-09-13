#include <stdio.h>  // FILE, fopen, fclose, etc.
#include <string.h> // strtok, strcpy, etc.
#include <stdlib.h>
#include <unistd.h>
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
  /*
  Queue* cola = queue_init();

  int primero;
  primero = 10;

  int segundo;
  segundo = 11;
  int tercero;
  tercero = 23;
  queue_insert(cola, primero);
  queue_insert(cola, segundo);
  queue_insert(cola, tercero);

  queue_print(cola);

  queue_destroy(cola);
  */

  pid_t pids[10];
  int i;
  for (i = 9; i >= 0; --i) {
    pids[i] = fork();
    if (pids[i] == 0) {
      sleep(i+1);
      _exit(0);
    }
  }
  for (i = 9; i >= 0; --i)
    waitpid(pids[i], NULL, 0);
  return 0;


}
