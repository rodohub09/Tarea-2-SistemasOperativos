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

void monitor_inicializar(Monitor *m, int n){
  pthread_mutex_init(&m->mutex, NULL);
  pthread_cond_init(&m->cond, NULL);
  m->count = 0;
  m->N = n;
  m->etapa = 1;
}

void monitor_wait(Monitor *m){
  pthread_mutex_lock(&m->mutex);
  m->count++;
  int mi_etapa = m->etapa;
  
  if (m->count == m->N) {
    m->count = 0;
    m->etapa++; 
    pthread_cond_broadcast(&m->cond); 
    
  } else {
    while (mi_etapa == m->etapa) {
      pthread_cond_wait(&m->cond, &m->mutex);
    }
  }

  pthread_mutex_unlock(&m->mutex);
}

void monitor_destroy(Monitor *m) {
    pthread_mutex_destroy(&m->mutex);
    pthread_cond_destroy(&m->cond);
}

