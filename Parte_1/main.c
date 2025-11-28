#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "monitor.h"

void* ejec_hebras(void* arg){
  DatosHebra *mis_datos = (DatosHebra*) arg;
  Monitor *m = mis_datos->monitor;
  
  for (int e = 1; e <= mis_datos->etapas; e++) {
    usleep(rand() % 100);
    printf("[Hebra %ld] esperando en etapa %d\n", pthread_self(), e);
    monitor_wait(m);
    printf("[Hebra %ld] pasó barrera en etapa %d\n", pthread_self(), e);
  }
  
  return NULL;
}

int main(int argc, char *argv[]){
  int hebras = 5;
  int etapas = 4;
  
  switch(argc){
  case 2:
    hebras = atoi(argv[1]);
    break;
  case 3:
    hebras = atoi(argv[1]);
    etapas = atoi(argv[2]);
    break;
  default: break;
  };
  
  Monitor monitor;

  monitor_inicializar(&monitor,hebras);
  
  pthread_t hilos[hebras];
  DatosHebra args[hebras];
  
  for (int i = 0; i < hebras; i++) {
    args[i].etapas = etapas;
    args[i].monitor = &monitor;
    pthread_create(&hilos[i], NULL, ejec_hebras, &args[i]);
  } 
  
  for (int i = 0; i < hebras; i++) {
    pthread_join(hilos[i], NULL);
  }
  
  monitor_destroy(&monitor);
  
  return 0;
}
