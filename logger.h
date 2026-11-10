#ifndef LOGGER_H
#define LOGGER_H

#include "fat.h"

// Borra el contenido del txt al abrir el programa
void resetear_registro();

// Escribe el estado de las 3 tablas en el txt
void volcar_tablas_a_txt(Sistema_FAT *fs, const char *accion);

#endif