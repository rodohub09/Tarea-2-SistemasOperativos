#ifndef MONITOR_H
#define MONITOR_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

typedef struct {
  int count;
  int N;
  int etapa;

  pthread_mutex_t mutex;
  pthread_cond_t cond;
  
} Monitor;

typedef struct {
    int etapas;
    Monitor *monitor;
} DatosHebra;

void monitor_inicializar(Monitor *m, int n);

void monitor_wait(Monitor *m);

void monitor_destroy(Monitor *m);

#endif
