#include <stdio.h>
#include "logger.h"

void resetear_registro() {
    // Al usar "w", el archivo se sobreescribe y queda en blanco cada vez que abres el programa
    FILE *archivo = fopen("registro_fat.txt", "w");
    if (archivo != NULL) {
        fprintf(archivo, "=== INICIO DEL SISTEMA FAT ===\n\n");
        fclose(archivo);
    }
}

void volcar_tablas_a_txt(Sistema_FAT *fs, const char *accion) {
    FILE *archivo = fopen("registro_fat.txt", "a");
    if (archivo == NULL) return;

    fprintf(archivo, "========================================\n");
    fprintf(archivo, "ACCION: %s\n", accion);
    fprintf(archivo, "========================================\n\n");

    // 1. TABLA DE BITS
    fprintf(archivo, "--- TABLA DE BITS (100 sectores) ---\n");
    for (int i = 0; i < max_sectores; i++) {
        fprintf(archivo, "[%d]", fs->tabla_bits[i].ocupado);
        if ((i + 1) % 10 == 0) fprintf(archivo, "\n");
    }
    fprintf(archivo, "\n");

    // 2. TABLA DE INDICE
    fprintf(archivo, "--- TABLA DE INDICE ---\n");
    fprintf(archivo, "%-5s | %-20s | %-6s | %-8s | %-6s | %-7s\n", "Ind.", "Nombre", "Inicio", "Bloques", "Bytes", "Borrado");
    fprintf(archivo, "----------------------------------------------------------------------\n");
    for (int i = 0; i < fs->cantidad_archivos; i++) {
        fprintf(archivo, "%-5d | %-20s | %-6d | %-8d | %-6d | %-7d\n",
            i,
            fs->tabla_indice[i].nombre_archivo,
            fs->tabla_indice[i].bloque_inicio,
            fs->tabla_indice[i].cantidad_bloques,
            fs->tabla_indice[i].tamano_bytes,
            fs->tabla_indice[i].borrado);
    }
    if (fs->cantidad_archivos == 0) fprintf(archivo, "Vacia.\n");
    fprintf(archivo, "\n");

    // 3. TABLA DE DATOS
    fprintf(archivo, "--- TABLA DE DATOS ---\n");
    fprintf(archivo, "%-5s | %-12s | %-16s\n", "Sec.", "Datos", "Bloque Siguiente");
    fprintf(archivo, "----------------------------------------\n");
    for (int i = 0; i < max_sectores; i++) {
        fprintf(archivo, "%-5d | '", i);
        // Imprimir los caracteres (si no hay nada, imprimimos un punto para que se vea la estructura)
        for (int j = 0; j < tam_bloque; j++) {
            if (fs->tabla_datos[i].datos[j] >= 32 && fs->tabla_datos[i].datos[j] <= 126) {
                fputc(fs->tabla_datos[i].datos[j], archivo);
            } else {
                fputc('.', archivo);
            }
        }
        fprintf(archivo, "' | %d\n", fs->tabla_datos[i].bloque_siguiente);
    }
    fprintf(archivo, "\n\n");

    fclose(archivo);
}