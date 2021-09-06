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

int pid_semaforo_A = 0;
int pid_semaforo_B = 0;
int pid_semaforo_C = 0;



int detener_procesos(int sig){
  printf("\n[MAIN] CIERRE FORZADO -- PID: %i\n", getpid());
  kill(pid_semaforo_A , SIGABRT);
  kill(pid_semaforo_B , SIGABRT);
  kill(pid_semaforo_C , SIGABRT);
}    

void guardar_pid_semaforo(int sig, siginfo_t *siginfo, void *context){
  int valor_aux = siginfo->si_value.sival_int;
  if (!pid_semaforo_A){
    pid_semaforo_A = valor_aux;
  }
  else if (!pid_semaforo_B){
    pid_semaforo_B = valor_aux;
  }
  else if (!pid_semaforo_C){
    pid_semaforo_C = valor_aux;
  }
}

void handle_prueba( int sig, siginfo_t *siginfo, void *context)
  {
    printf("\n > > > > > Entro a la funcion\n");
    pid_fabrica = siginfo->si_value.sival_int;
    printf("\n\n > > > > > el numero recibido es: %i\n", pid_fabrica);    
  }

void fabrica(int n_repartidores, char* semaforo1, char* semaforo2, char* semaforo3, char* bodega)
{

  void test_handle_repartidor(int test_info, siginfo_t *siginfo, void *context)
  {
    printf("Hola repartidor %d\n", siginfo->si_value.sival_int);
  }
  connect_sigaction(SIGUSR1, &test_handle_repartidor);

  printf("\nIniciando la FABRICA . . .\n[FABRICA] pid: %i - PADRE: %i\n\n", getpid(), getppid());
  printf("N repartidores fabrica %d\n", n_repartidores);
  for (int j = 1; j <= n_repartidores; j++)
  {
    char repartidor_id[2];
    sprintf(repartidor_id, "%c", j);

    pid = fork();
    if (!pid){
      execl("./repartidor", repartidor_id, semaforo1, semaforo2, semaforo3,
        bodega, NULL);
    }
    else {}
  }

  int ubicacion_s1 = atoi(semaforo1);
  int ubicacion_s2 = atoi(semaforo2);
  int ubicacion_s3 = atoi(semaforo3);
  int ubicacion_b = atoi(bodega);

  int auxiliar_signal = 2; // test de comunicaciones


  /*printf("[Fabrica] S1-%d ; S2-%d ; S3-%d ; B-%d\n",ubicacion_s1, ubicacion_s2,
    ubicacion_s3, ubicacion_b); */

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

    char * aux_string =  data_in->lines[1][1];
    int aux_repartidor = atoi(aux_string);

    char * semaforo1 = data_in->lines[0][0];
    char * semaforo2 = data_in->lines[0][1];
    char * semaforo3 = data_in->lines[0][2];
    char * bodega = data_in->lines[0][3];
    printf("---\n");

    printf("--- Numero repartidores aux integer %d\n", aux_repartidor);
    fabrica(aux_repartidor, semaforo1, semaforo2, semaforo3, bodega);

  } else {

    connect_sigaction(SIGUSR1, handle_prueba);
    
    sleep(1);
    printf("\n\n - - - > El lçvalor es:");
    printf(" %i\n", pid_fabrica);


    // codigo para transformar un INT a STR
    int length_1 = snprintf(NULL, 0, "%d", pid_fabrica); //calcula el largo del int (espacio en memoria)
    unsigned int numero = pid_fabrica;  //  unsigned int
    char numero_str[length_1 + 1];
    sprintf(numero_str, "%u", numero); // (espacio donde se guardara, tipo de dato, int a transformar)u es para un unsigned int
    printf("\n > > > > > >  > IMPRIME: %s\n", numero_str);

 
    


    /* Intento de crear una funcion  */          /*
    int largo_aux = snprintf(NULL, 0, "%d", num); //calcula el largo del int (espacio en memoria)
    unsigned int numero_aux = num;  //  unsigned int
    char espacio_reservado[largo_aux + 1];
    sprintf(espacio_reservado, "%u", numero_aux); // (espacio donde se guardara, tipo de dato, int a transformar)u es para un unsigned int

    printf("\n > > > > > >  > Probando utilidad de la funcion\n%s", var);
  */

    // Continuacion con el proceso principal      
    for (int i = 0; i < 3; i+=1)
    {

      // aqui probamos el cambio de variable de int  a srt
      int largo_aux = snprintf(NULL, 0, "%d", i); //calcula el largo del int (espacio en memoria)
      unsigned int numero_aux = i;  //  unsigned int
      char id_semaforo[largo_aux + 1];
      sprintf(id_semaforo, "%u", numero_aux); // (espacio donde se guardara, tipo de dato, int a transformar)u es para un unsigned int

      //printf("\n > > > > > >  > Probando utilidad de la funcion ->%s\n", id_semaforo);

      // Aqui el proceso main crea los procesos semaforos
      pid = fork();
      if (pid == 0){ 
        send_signal_with_int( getppid() , getpid() ); // señal que envia
        execl("./semaforo",id_semaforo, data_in->lines[0][i],numero_str, NULL); 
        
      } 


      //=========================================================================
      // [MAIN]: DESDE AQUI ES SOLO PROCESO MAIN
      else{
        //SEÑAL para finalizar todo( CTRL+C)
        signal(SIGINT, &detener_procesos); 
        // SEÑAL PARA ENVIAR EL PID DE SEMAFORO
        connect_sigaction(SIGUSR1, guardar_pid_semaforo);
        printf("\nLLego al sleep\n");
        sleep(2);

        


      }
      
    }   
   
    
    
    
    
      

    for( int i = 0; i < 4; i++){
      pid = wait(&status);
      printf("[INNIT] finalizando  - padre: %d - hijo: %d\n", getpid(), pid);
    } 

    
  }  
  
  
  return 0;
}
