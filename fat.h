#ifndef FAT_H
#define FAT_H

#define max_sectores 100
#define max_archivos 100
#define tam_bloque 10
#define fin_archivo -1

typedef struct {
    char nombre_archivo[30]; // 30 para dar espacio a los (1), (2) en duplicados
    int bloque_inicio;
    int cantidad_bloques;
    int tamano_bytes;        // Agregado para saber los bytes reales
    int borrado;             // 0 = Activo, 1 = Borrado
} Tabla_indice;

typedef struct {
    char datos[tam_bloque];
    int bloque_siguiente;
} Tabla_datos;

typedef struct {
    int ocupado;             // 0 = Libre, 1 = Ocupado
} Tabla_bit;

// Contenedor principal del disco
typedef struct {
    Tabla_indice tabla_indice[max_archivos];
    Tabla_datos tabla_datos[max_sectores];
    Tabla_bit tabla_bits[max_sectores];
    int cantidad_archivos;
} Sistema_FAT;

// Funciones principales
void inicializar(Sistema_FAT *fs);
void agregar(Sistema_FAT *fs, const char *nombre_crudo, const char *contenido);
void mostrar(Sistema_FAT *fs, const char *nombre); // Equivale a "leer archivo"
void listar(Sistema_FAT *fs);
void borrar(Sistema_FAT *fs, const char *nombre);
void mostrar_matriz(Sistema_FAT *fs);

#endif