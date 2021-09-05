#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

#include "../file_manager/manager.h"

//  :jack_o_lantern:

// Variables
pid_t pid;
int status;
int pid_fabrica;

void handle_prueba( int sig, siginfo_t *siginfo, void *context)
  {
    printf("\n > > > > > Entro a la funcion\n");
    pid_fabrica = siginfo->si_value.sival_int;
    printf("\n\n > > > > > el numero recibido es: %i\n", pid_fabrica);    
  }

void fabrica(int n_repartidores)
{
  printf("\nIniciando la FABRICA . . .\n[FABRICA] pid: %i - PADRE: %i\n\n", getpid(), getppid());
  printf("\n repartidores fabrica %d\n", n_repartidores);
  for (int j = 0; j < n_repartidores; j++)
  {
    pid = fork();
    if (!pid){
      execl("./repartidor", NULL);
    }
    else {}
  }
  
  for( int i = 0; i < n_repartidores; i++){
      pid = wait(&status);
      printf("[INNIT] finalizando  - padre: %d - hijo: %d\n", getpid(), pid);
    }

}

int main(int argc, char const *argv[])
{ 


  printf("\n[PRINCIPAL]: my PID is: %i\n", getpid());

    char *filename = argv[1];
    InputFile *data_in = read_file(filename);

    printf("Leyendo el archivo %s...\n", filename);
    printf("- Lineas en archivo: %i\n", data_in->len);
    printf("- Contenido del archivo:\n");
  
  // Parseo de archivo

    printf("Liberando memoria...\n");
    input_file_destroy(data_in);
    



  // Iniciacion de FABRICA  
  pid = fork();
  
  if (!pid){
    //int dir_fabrica = getpid() ;

    send_signal_with_int( getppid() , getpid() ); // señal que envia

    char * aux_string =  data_in->lines[1][1];
    int aux_repartidor = atoi(aux_string);
    fabrica(aux_repartidor);

  } else {

    connect_sigaction(SIGUSR1, handle_prueba);
    
    sleep(5);
    printf("\n\n - - - > El lçvalor es:");
    printf(" %i\n", pid_fabrica);
    
    /*
    int length = snprintf(NULL, 0, "%d", pid_fabrica);
    char str[length+1];
    snprintf(str, "%d", pid_fabrica);



    int length = snprintf(NULL, 0, "%d", pid_fabrica);
    char numero_str[length];
    sprintf(numero_str, "%c", pid_fabrica);
    printf("\n\nQues es esto: %c\n", numero_str);
    */

    int length_1 = snprintf(NULL, 0, "%d", pid_fabrica);
    unsigned int numero = pid_fabrica;
    char numero_str[length_1 + 1];
    sprintf(numero_str, "%u", numero);

    printf("\n > > > > > >  > IMPRIME: %s\n", numero_str);
 
    


    // Continuacion con el proceso principal      
    for (int i = 0; i < 3; i+=1)
    {
      // Aqui el proceso main crea los procesos semaforos
      pid = fork();
      if (pid == 0){ 
        
        execl("./semaforo", data_in->lines[0][i],numero_str, NULL); 
        
      } 
      
    } 
    
   
    
    


    for( int i = 0; i < 4; i++){
      pid = wait(&status);
      printf("[INNIT] finalizando  - padre: %d - hijo: %d\n", getpid(), pid);
    } 

    
  }  
  
  
  return 0;
}
