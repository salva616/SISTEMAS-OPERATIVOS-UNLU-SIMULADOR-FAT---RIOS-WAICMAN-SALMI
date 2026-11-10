#ifndef UTILS_H
#define UTILS_H

#include "fat.h"

void formatear_nombre(char *nombre);
void resolver_duplicado(Sistema_FAT *fs, char *nombre);
int contar_bloques_libres(Sistema_FAT *fs);
int buscar_bloque_libre(Sistema_FAT *fs);
int buscar_indice_archivo(Sistema_FAT *fs, const char *nombre);
void limpiar_buffer();

// Nuevas funciones de interfaz
void limpiar_pantalla();
void esperar_tecla();

#endif