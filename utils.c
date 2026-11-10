#include <stdio.h>
#include <stdlib.h> // Necesario para system()
#include <string.h>
#include "utils.h"

void formatear_nombre(char *nombre) {
    for (int i = 0; nombre[i] != '\0'; i++) {
        if (nombre[i] == ' ') {
            nombre[i] = '_';
        }
    }
}

int buscar_indice_archivo(Sistema_FAT *fs, const char *nombre) {
    for (int i = 0; i < fs->cantidad_archivos; i++) {
        if (fs->tabla_indice[i].borrado == 0 && strcmp(fs->tabla_indice[i].nombre_archivo, nombre) == 0) {
            return i;
        }
    }
    return -1;
}

void resolver_duplicado(Sistema_FAT *fs, char *nombre) {
    char nombre_base[30];
    strcpy(nombre_base, nombre);
    int contador = 1;
    
    while (buscar_indice_archivo(fs, nombre) != -1) {
        snprintf(nombre, 30, "%s(%d)", nombre_base, contador);
        contador++;
    }
}

int contar_bloques_libres(Sistema_FAT *fs) {
    int libres = 0;
    for (int i = 0; i < max_sectores; i++) {
        if (fs->tabla_bits[i].ocupado == 0) libres++;
    }
    return libres;
}

int buscar_bloque_libre(Sistema_FAT *fs) {
    for (int i = 0; i < max_sectores; i++) {
        if (fs->tabla_bits[i].ocupado == 0) return i;
    }
    return -1;
}

void limpiar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void limpiar_pantalla() {
    // Detecta si estás en Windows (MinGW/GCC) o Linux/Mac
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void esperar_tecla() {
    printf("\nPresione ENTER para continuar...");
    getchar(); // Espera a que el usuario presione una tecla
}