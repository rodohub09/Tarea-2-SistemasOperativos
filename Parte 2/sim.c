#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "funciones.h"

int main(int argc, char* argv[]){
  if(argc < 4) {
      printf("Error: Faltan argumentos.\nUso: ./sim <n_marcos> <tam_pagina> <archivo>\n");
      return 1;
  }
  
  int marcos = atoi(argv[1]);
  int size_param = atoi(argv[2]);
  if (marcos <= 0 || size_param <= 0) {
      printf("Error: Los argumentos deben ser positivos.\n");
      return 1;
  }

  int verbose = 0; 
  char* nombre_archivo = NULL;

  if (strcmp(argv[3], "-verbose") == 0 || strcmp(argv[3], "--verbose") == 0) {
      verbose = 1;
      if (argc < 5) {
          printf("Error: Se especificó -verbose pero falta el archivo.\n");
          return 1;
      }
      nombre_archivo = argv[4];
  } else {
      verbose = 0;
      nombre_archivo = argv[3];
  }
  
  uint32_t PAGE_SIZE = (uint32_t)size_param;
  uint32_t MASK = PAGE_SIZE - 1;
  int b = 0;
  uint32_t temp = PAGE_SIZE;
  
  Marco *tabla = (Marco*)malloc(sizeof(Marco) * marcos);
  if (tabla == NULL) {
      printf("Error: No hay memoria suficiente.\n");
      return 1;
  }

  int manecilla = 0;
  int ocupados = 0;

  // Inicialización
  for(int i = 0; i < marcos; i++){
    tabla[i].id = i;
    tabla[i].npv = -1;
    tabla[i].libre = 1;
    tabla[i].bit_ref = 0;
  }
  
  // Calcular bits de shift (b)
  while (temp > 1) {
    temp >>= 1;
    b++;
  }

  FILE *archivo = fopen(nombre_archivo, "r"); 
  if (!archivo) { 
      perror("Error abriendo archivo"); 
      free(tabla); // Liberar memoria antes de salir
      return 1; 
  }
  float referencias = 0;
  float fallos = 0;
  uint32_t va;
  
  // Lectura del archivo
  while(fscanf(archivo, "%x", &va) != EOF){
    referencias++;
    uint32_t offset = va & MASK;
    uint32_t npv = va >> b;
    char* hit;
    
    int marcoID = buscar_pagina_en_memoria(tabla, marcos, npv);
    
    if(marcoID == -1) {
      // CASO FALLO:
      hit = "FALLO";
      fallos++;
      manejar_fallo(&manecilla, &ocupados, tabla, marcos, npv);
      marcoID = buscar_pagina_en_memoria(tabla, marcos, npv);
    
    } else {
      // CASO HIT: Actualizar bit referencia
      hit = "HIT";
      tabla[marcoID].bit_ref = 1;
    }
    uint32_t DF = (marcoID << b) | offset;
    
    if(verbose) {
      printf("DV: %#X | NPV: %#X | OFFSET: %#X | %s | DF: %#X\n", va, npv, offset, hit, DF); 
    }
  }
  
  fclose(archivo);
  free(tabla); // Liberar memoria

  printf("Referencias: %.0f | Fallos: %.0f | Tasa de fallos: %.2f%c\n", referencias, fallos, (fallos/referencias)*100, 37);
  
  return 0;
}
