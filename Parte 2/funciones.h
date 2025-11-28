#ifndef MONITOR_H
#define MONITOR_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct {
  int id;
  int npv;
  int bit_ref;
  int libre;
} Marco;

int buscar_pagina_en_memoria(Marco tabla[], int marcos, uint32_t npv);
void reloj(int *manecilla, Marco tabla[], int marcos, uint32_t npv);
void manejar_fallo(int *manecilla, int *ocupados, Marco tabla[], int marcos, uint32_t npv);

#endif
