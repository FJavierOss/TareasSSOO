#include <stdio.h>
#include <unistd.h>

#include "../file_manager/manager.h"

//   :Jack-O-Lantern:


void handle_prueba( int sig, siginfo_t *siginfo, void *context)
  {
    printf("Entro a la funcion\n");
    int number = siginfo->si_value.sival_int;
    printf("el numero recibido es: %i\n", number);
    
  }

int main(int argc, char const *argv[])
{ 
  printf("I'm the DCCUBER process and my PID is: %i\n", getpid());

  // Variables
  pid_t pid;
  int status;


  
  char *filename = argv[1];
  InputFile *data_in = read_file(filename);

  printf("Leyendo el archivo %s...\n", filename);
  printf("- Lineas en archivo: %i\n", data_in->len);
  printf("- Contenido del archivo:\n");

  printf("\t- ");
  for (int i = 0; i < 3; i++)
  {

    char* aux = data_in->lines[0][i];
    pid = fork();

    // Este es el HIJO
    if (pid == 0){
      printf("\n[HIJO] %d \n", pid);      
      execl("./semaforo", aux, NULL);   
    } 

    // Este es el PADRE
    printf("\n[MAIN] %d \n", pid);
    
    
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

  for( int i = 0; i < 3; i++){
    pid = wait(&status);
    printf("finalizo INNIT - padre: %d - hijo: %d\n", getpid(), pid);
  } 
  
  return 0;
}
