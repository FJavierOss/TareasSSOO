#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include "../file_manager/manager.h"
#include <time.h> // para sleep



//  :jack_o_lantern:

// Variables
pid_t pid;
int status;
int pid_fabrica = 0;

int pid_semaforo_A = 0;
int pid_semaforo_B = 0;
int pid_semaforo_C = 0;




int detener_procesos(int sig){
  printf("\n[MAIN] CIERRE FORZADO -- PID: %i\n", getpid());
  kill(pid_semaforo_A , SIGABRT);
  kill(pid_semaforo_B , SIGABRT);
  kill(pid_semaforo_C , SIGABRT);
  kill(pid_fabrica , SIGABRT);
}

void MySignailHandler(int sig){
 printf("\n[FABRICA] No hace nada\n");
}







void handle_prueba( int sig, siginfo_t *siginfo, void *context)
  {
    printf("Entro a la funcion\n");
    int number = siginfo->si_value.sival_int;
    printf("el numero recibido es: %i\n", number);

  }

void fabrica(int n_repartidores, char* semaforo1, char* semaforo2, char* semaforo3, char* bodega, int delay_repartidor)
{
  // Armamos un array de ints para guardar los pid de los repartidores
  // los inicializamos en 0 para que cuando se informen a los repartidores el
  // cambio de un semaforo se evite mandarle señales a procesos que no tienen
  // que ver con la tarea
  int *lista_repartidores = calloc(n_repartidores+1, sizeof(int));
  printf("\n\n\n> > > > > > > %i \n", delay_repartidor);
 
 
  void funcion_termino_fabrica(int sig){
    for (int indicador = 0; indicador < n_repartidores; indicador++){
      if (lista_repartidores[indicador]){
        kill(lista_repartidores[indicador] , SIGABRT);
      }
      
    }
  }


  // Definimos los estados de los semaforos,  como la fábrica se crea antes
  // que los semaforos, asumimos que parten en verde como dice el enunciado
  // verde = 1, rojo = 0

  int estado_s1 = 1;
  int estado_s2 = 1;
  int estado_s3 = 1;

  // señal de termino
  signal(SIGABRT, funcion_termino_fabrica);


  



  void handle_fabrica(int test_info, siginfo_t *siginfo, void *context)
  {

    // Vamos a reservar las señales de los semaforo sobre sus estados en los
    // número menores que 10, toda señal que llegue con un número mayor a 10
    // se considerara que es un pid de un repartidor,(estamos asumiendo que
    // el computador tiene suficientes procesos corriendo y que nunca deberia
    // haber un repartidor con un pid menor a 10)

    if (siginfo->si_value.sival_int < 10){

      // Los valores 1 y 2 estan reservados para el estado del semaforo 1,
      // Los valores 3 y 4 estan reservados para el estado del semaforo 2,
      // Los valores 5 y 6 estan reservados para el estado del semaforo 3

      if (siginfo->si_value.sival_int == 1){
        estado_s1 = 0;

      } else if (siginfo->si_value.sival_int == 2){
        estado_s1 = 1;

      } else if (siginfo->si_value.sival_int == 3){
        estado_s2 = 0;

      } else if (siginfo->si_value.sival_int == 4){
        estado_s2 = 1;

      } else if (siginfo->si_value.sival_int == 5){
        estado_s3 = 0;

      } else if (siginfo->si_value.sival_int == 6){
        estado_s3 = 1;

      }

      printf("Recibiendo señal del semaforo %d ------------\n", siginfo->si_value.sival_int);

      // Actualizamos el estado de los semaforos a todos los hijos
      for (int i = 0; i < n_repartidores; i++){

        // Seguridad para no mandar señales a procesos desconocidos segun lo
        // definido anteriormente
        if (lista_repartidores[i] > 0){
          send_signal_with_int(lista_repartidores[i], siginfo->si_value.sival_int);
        }

      }
    }
  }


  connect_sigaction(SIGUSR1, &handle_fabrica);

  // Esperamos que se creen los semaforos

  printf("\nIniciando la FABRICA . . .\n[FABRICA] pid: %i - PADRE: %i\n\n", getpid(), getppid());
  printf("N repartidores fabrica %d\n", n_repartidores);
  sleep(1);

  for (int j = 1; j < n_repartidores+1; j++)
  {
    
    char repartidor_id[2];
    sprintf(repartidor_id, "%c", j);

    char estado_s1_[2];
    sprintf(estado_s1_, "%c", estado_s1);

    char estado_s2_[2];
    sprintf(estado_s2_, "%c", estado_s2);

    char estado_s3_[2];
    sprintf(estado_s3_, "%c", estado_s3);

    pid = fork();
    if (!pid){
      printf("\n\n[REPARTIDOR] New -PID: %i\n", getpid());
      printf("Repartidor ID: %d\n", repartidor_id[0]);

      execl("./repartidor", repartidor_id, semaforo1, semaforo2, semaforo3,
        bodega, estado_s1_, estado_s2_, estado_s3_, NULL);
    }
    else {
       // Guardamos el pid del hijo en el array
      lista_repartidores[j-1] = pid;
      int demora = sleep(delay_repartidor); 
      while (demora){
        demora = sleep(demora);
      }

     }

    printf("[");
    for (int k = 0; k < n_repartidores; k++){
      printf("%d,", lista_repartidores[k]);
    }
    printf("]\n" );

    
  }

  int ubicacion_s1 = atoi(semaforo1);
  int ubicacion_s2 = atoi(semaforo2);
  int ubicacion_s3 = atoi(semaforo3);
  int ubicacion_b = atoi(bodega);

  int auxiliar_signal = 2; // test de comunicaciones


  /*printf("[Fabrica] S1-%d ; S2-%d ; S3-%d ; B-%d\n",ubicacion_s1, ubicacion_s2,
    ubicacion_s3, ubicacion_b); */

  printf("--------------------------------------------------------------------##\n");
  int contador_repartidores = 0;
  while( contador_repartidores < n_repartidores){
    pid = wait(&status);
    for (int i = 0; i < n_repartidores; i++){
      if (pid == lista_repartidores[i]){
        lista_repartidores[i] = 0;
        contador_repartidores += 1;
      }
    }
    printf("[INNIT] finalizando  - padre: %d - hijo: %d\n", getpid(), pid);
  }
  printf("--------------------------------------------------------------------$$\n");
  printf("Liberando memoria ...\n");
  free(lista_repartidores);
  
  

}



int main(int argc, char const *argv[])
{


  printf("\n[PRINCIPAL]: my PID is: %i\n", getpid());

    char *filename = argv[1];
    InputFile *data_in = read_file(filename);

    printf("Leyendo el archivo %s...\n", filename);
    printf("- Lineas en archivo: %i\n", data_in->len);
    printf("- Contenido del archivo:\n");


  // Iniciacion de FABRICA
  pid_fabrica = fork();
  
  signal(SIGINT, MySignailHandler);
  //signal(SIGABRT, funcion_termino_fabrica);

  if (!pid_fabrica){
    char * aux_string =  data_in->lines[1][1];
    int aux_repartidor = atoi(aux_string);
    int delay_repartidor = atoi(data_in->lines[1][0]);
    char * semaforo1 = data_in->lines[0][0];
    char * semaforo2 = data_in->lines[0][1];
    char * semaforo3 = data_in->lines[0][2];
    char * bodega = data_in->lines[0][3];
    //printf("---Pid Main %d  Pid Fabrica %d\n", getppid(), getpid());
    //printf("--- Numero repartidores aux integer %d\n", aux_repartidor);
    fabrica(aux_repartidor, semaforo1, semaforo2, semaforo3, bodega, delay_repartidor);

  } else {

    // Continuacion con el proceso principal

    // Aquí convertimos el pid de la fabrica de int a str para posterior envio
    // a los semaforos en su creacion
    int length_1 = snprintf(NULL, 0, "%d", pid_fabrica);
    unsigned int numero = pid_fabrica;
    char numero_str[length_1 + 1];
    sprintf(numero_str, "%u", numero);
    printf("\n > > > > > >  > IMPRIME: %s %d\n", numero_str, pid_fabrica);


    // Aqui se crearan los 3 semaforos
    for (int i = 0; i < 3; i++)
    {
      // Procesamos el identificador del semaforo para que pueda ser enviado en execl
      int largo_aux = snprintf(NULL, 0, "%d", i); //calcula el largo del int (espacio en memoria)
      unsigned int numero_aux = i;  //  unsigned int
      char id_semaforo[largo_aux + 1];
      sprintf(id_semaforo, "%u", numero_aux);

      // Aqui el proceso main crea los procesos semaforos
      pid = fork();
      if (pid == 0){

        execl("./semaforo", id_semaforo, data_in->lines[1][i+2], numero_str, NULL);

      } else {
        if (i == 0){
          pid_semaforo_A = pid;
        } else if (i == 1){
          pid_semaforo_B = pid;
        } else if (i == 2){
          pid_semaforo_C = pid;
        }


        // Se continua con el proceso Main
        //SEÑAL para finalizar todo( CTRL+C)
        signal(SIGINT, &detener_procesos);
      }
    }


    // Se espera a que los hijos terminen
    for( int i = 0; i < 4; i++){
      pid = wait(&status);
      if (pid == pid_fabrica){
        detener_procesos(10);
      }
      printf("[INNIT] finalizando  - padre: %d - hijo: %d\n", getpid(), pid);
    }
  }

  printf("Liberando memoria...\n");
  input_file_destroy(data_in);



  return 0;
}