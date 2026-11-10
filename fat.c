#include <stdio.h>
#include <string.h>
#include <math.h>
#include "fat.h"
#include "utils.h"

void inicializar(Sistema_FAT *fs) {
    fs->cantidad_archivos = 0;
    
    // 100 ceros antes de empezar
    for (int i = 0; i < max_sectores; i++) {
        fs->tabla_bits[i].ocupado = 0;
        fs->tabla_datos[i].bloque_siguiente = fin_archivo;
        memset(fs->tabla_datos[i].datos, 0, tam_bloque);
    }
    
    for (int i = 0; i < max_archivos; i++) {
        fs->tabla_indice[i].borrado = 1; 
    }
}

void agregar(Sistema_FAT *fs, const char *nombre_crudo, const char *contenido) {
    if (fs->cantidad_archivos >= max_archivos) {
        printf("Error: Directorio raiz lleno.\n");
        return;
    }

    char nombre[30];
    strncpy(nombre, nombre_crudo, 25);
    nombre[25] = '\0';
    
    formatear_nombre(nombre);
    resolver_duplicado(fs, nombre);

    int longitud_bytes = strlen(contenido);
    
    // Divide por 10 y redondea hacia arriba
    int bloques_necesarios = (int)ceil((double)longitud_bytes / tam_bloque);
    if (bloques_necesarios == 0) bloques_necesarios = 1; 

    if (contar_bloques_libres(fs) < bloques_necesarios) {
        printf("Error: Espacio insuficiente. Se requieren %d sectores.\n", bloques_necesarios);
        return;
    }

    int primer_bloque = -1;
    int bloque_anterior = -1;
    int indice_contenido = 0;

    for (int i = 0; i < bloques_necesarios; i++) {
        int bloque_libre = buscar_bloque_libre(fs);
        
        fs->tabla_bits[bloque_libre].ocupado = 1; // Cambia el valor binario a 1

        if (primer_bloque == -1) primer_bloque = bloque_libre;
        if (bloque_anterior != -1) fs->tabla_datos[bloque_anterior].bloque_siguiente = bloque_libre;

        memset(fs->tabla_datos[bloque_libre].datos, 0, tam_bloque);
        for(int j = 0; j < tam_bloque && indice_contenido < longitud_bytes; j++, indice_contenido++) {
            fs->tabla_datos[bloque_libre].datos[j] = contenido[indice_contenido];
        }
        
        fs->tabla_datos[bloque_libre].bloque_siguiente = fin_archivo;
        bloque_anterior = bloque_libre;
    }

    int idx = fs->cantidad_archivos;
    strcpy(fs->tabla_indice[idx].nombre_archivo, nombre);
    fs->tabla_indice[idx].bloque_inicio = primer_bloque;
    fs->tabla_indice[idx].cantidad_bloques = bloques_necesarios;
    fs->tabla_indice[idx].tamano_bytes = longitud_bytes; 
    fs->tabla_indice[idx].borrado = 0;
    
    fs->cantidad_archivos++;

    printf("Archivo '%s' guardado exitosamente (%d bytes en %d sectores).\n", nombre, longitud_bytes, bloques_necesarios);
}

void mostrar(Sistema_FAT *fs, const char *nombre) {
    int idx = buscar_indice_archivo(fs, nombre);
    if (idx == -1) {
        printf("El archivo '%s' no existe.\n", nombre); 
        return;
    }

    printf("Archivo: %s\n", fs->tabla_indice[idx].nombre_archivo);
    printf("Contenido: ");
    
    int bloque_actual = fs->tabla_indice[idx].bloque_inicio;
    int bloques_leidos = 0;
    
    while (bloque_actual != fin_archivo && bloques_leidos < fs->tabla_indice[idx].cantidad_bloques) {
        for(int i = 0; i < tam_bloque; i++) {
            if(fs->tabla_datos[bloque_actual].datos[i] != '\0') {
                putchar(fs->tabla_datos[bloque_actual].datos[i]);
            }
        }
        bloque_actual = fs->tabla_datos[bloque_actual].bloque_siguiente;
        bloques_leidos++;
    }
    printf("\n");
}

void listar(Sistema_FAT *fs) {
    printf("\n--- Lista de Archivos (Raiz) ---\n");
    int contador = 0;
    for (int i = 0; i < fs->cantidad_archivos; i++) {
        if (fs->tabla_indice[i].borrado == 0) {
            printf("- %s | Inicio: %d | Sectores: %d | Tamano: %d bytes\n", 
                fs->tabla_indice[i].nombre_archivo, 
                fs->tabla_indice[i].bloque_inicio, 
                fs->tabla_indice[i].cantidad_bloques,
                fs->tabla_indice[i].tamano_bytes);
            contador++;
        }
    }
    if (contador == 0) printf("El directorio esta vacio.\n");
    printf("--------------------------------\n");
}

void borrar(Sistema_FAT *fs, const char *nombre) {
    int idx = buscar_indice_archivo(fs, nombre);
    if (idx == -1) {
        printf("Error: El archivo '%s' no existe.\n", nombre);
        return;
    }

    int bloque_actual = fs->tabla_indice[idx].bloque_inicio;
    
    while (bloque_actual != fin_archivo) {
        fs->tabla_bits[bloque_actual].ocupado = 0; // Cambia el valor binario a 0
        int siguiente = fs->tabla_datos[bloque_actual].bloque_siguiente;
        fs->tabla_datos[bloque_actual].bloque_siguiente = fin_archivo; 
        bloque_actual = siguiente;
    }

    fs->tabla_indice[idx].borrado = 1;

    printf("Archivo '%s' borrado. Espacio liberado para sobreescritura.\n", nombre);
}

void mostrar_matriz(Sistema_FAT *fs) {
    // 1. MATRIZ DE FRAGMENTACION
    int mapa_archivos[max_sectores];
    for (int i = 0; i < max_sectores; i++) {
        mapa_archivos[i] = -1;
    }

    for (int i = 0; i < fs->cantidad_archivos; i++) {
        if (fs->tabla_indice[i].borrado == 0) {
            int bloque_actual = fs->tabla_indice[i].bloque_inicio;
            while (bloque_actual != fin_archivo) {
                mapa_archivos[bloque_actual] = i; 
                bloque_actual = fs->tabla_datos[bloque_actual].bloque_siguiente;
            }
        }
    }

    printf("--- Estado del Disco (Matriz 10x10) ---\n");
    for (int i = 0; i < max_sectores; i++) {
        if (fs->tabla_bits[i].ocupado == 1) {
            printf("[%02d]", mapa_archivos[i]); 
        } else {
            printf("[--]"); 
        }
        
        if ((i + 1) % 10 == 0) {
            printf("\n");
        }
    }
    
    printf("\n--- Leyenda de Archivos Activos ---\n");
    int contador = 0;
    for (int i = 0; i < fs->cantidad_archivos; i++) {
        if (fs->tabla_indice[i].borrado == 0) {
            printf("[%02d] = %s\n", i, fs->tabla_indice[i].nombre_archivo);
            contador++;
        }
    }
    if (contador == 0) printf("No hay archivos activos.\n");

    // 2. TABLA DE INDICE
    printf("\n--- TABLA DE INDICE ---\n");
    printf("%-5s | %-20s | %-6s | %-8s | %-6s | %-7s\n", "Ind.", "Nombre", "Inicio", "Bloques", "Bytes", "Borrado");
    printf("----------------------------------------------------------------------\n");
    for (int i = 0; i < fs->cantidad_archivos; i++) {
        printf("%-5d | %-20s | %-6d | %-8d | %-6d | %-7d\n",
            i,
            fs->tabla_indice[i].nombre_archivo,
            fs->tabla_indice[i].bloque_inicio,
            fs->tabla_indice[i].cantidad_bloques,
            fs->tabla_indice[i].tamano_bytes,
            fs->tabla_indice[i].borrado);
    }
    if (fs->cantidad_archivos == 0) printf("Vacia.\n");

    // 3. TABLA DE DATOS
    printf("\n--- TABLA DE DATOS (100 sectores) ---\n");
    printf("%-5s | %-12s | %-16s\n", "Sec.", "Datos", "Bloque Siguiente");
    printf("----------------------------------------\n");
    for (int i = 0; i < max_sectores; i++) {
        printf("%-5d | '", i);
        for (int j = 0; j < tam_bloque; j++) {
            // Imprime el caracter si es legible, sino un punto
            if (fs->tabla_datos[i].datos[j] >= 32 && fs->tabla_datos[i].datos[j] <= 126) {
                putchar(fs->tabla_datos[i].datos[j]);
            } else {
                putchar('.');
            }
        }
        printf("' | %d\n", fs->tabla_datos[i].bloque_siguiente);
    }
    printf("---------------------------------------\n");
}