#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct {
  int id;
  int npv;
  int bit_ref;
  int libre;
} Marco;

int buscar_pagina_en_memoria(Marco tabla[], int marcos, uint32_t npv) {
  for (int i = 0; i < marcos; i++) {
    // Verificamos !libre para asegurar que contiene datos válidos
    if (!tabla[i].libre && tabla[i].npv == npv) {
      return i; 
    }
  }
  return -1; 
}

void reloj(int *manecilla, Marco tabla[], int marcos, uint32_t npv) {
  // Mientras tenga bit de referencia en 1, dale segunda oportunidad
  while(tabla[*manecilla].bit_ref != 0){
    tabla[*manecilla].bit_ref = 0;
    (*manecilla)++;
    if(*manecilla >= marcos)
      *manecilla = 0;
  }
  
  // Encontró víctima (bit_ref == 0)
  tabla[*manecilla].npv = npv;
  tabla[*manecilla].bit_ref = 1;
  tabla[*manecilla].libre = 0;

  // Avanzar manecilla para la próxima vez
  (*manecilla)++;
  if(*manecilla >= marcos) *manecilla = 0;
}

void manejar_fallo(int *manecilla, int *ocupados, Marco tabla[], int marcos, uint32_t npv) {
  if (*ocupados < marcos) {
    // Caso: Aún hay espacio físico (Cold Start)
    int id_destino = *ocupados; 
    tabla[id_destino].npv = npv;
    tabla[id_destino].bit_ref = 1;
    tabla[id_destino].libre = 0;
    
    (*ocupados)++; 
  } else {
    // Caso: Memoria llena, invocar Algoritmo del Reloj
    reloj(manecilla, tabla, marcos, npv);
  }
}
