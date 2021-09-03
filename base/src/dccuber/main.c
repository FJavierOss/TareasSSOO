#include <stdio.h>
#include <unistd.h>

#include "../file_manager/manager.h"

int main(int argc, char const *argv[])
{
  printf("PRUEBA >>>>>>>>>>>>>>>>>>>>>>>>>>");
  printf("I'm the DCCUBER process and my PID is: %i\n", getpid());


  
  char *filename = argv[1];
  InputFile *data_in = read_file(filename);

  printf("Leyendo el archivo %s...\n", filename);
  printf("- Lineas en archivo: %i\n", data_in->len);
  printf("- Contenido del archivo:\n");

  printf("\t- ");
  for (int i = 0; i < 3; i++)
  {
    printf("\nDONDE ESTA? %s ", data_in->lines[0][i]);
    char* aux = data_in->lines[0][i];
    int pid = fork();
    if (pid == 0){
      printf("\n%d esto es hijo\n", pid);
      execl("./semaforo", aux, NULL);
    } else {
      printf("\n %d soy main\n", pid);
    }
    
  }
  printf("\n");

  printf("\t- ");
  for (int i = 0; i < 5; i++)
  {
    printf("%s, ", data_in->lines[1][i]);
  }
  printf("\n");

  printf("Liberando memoria...\n");
  input_file_destroy(data_in);

  
  
}
