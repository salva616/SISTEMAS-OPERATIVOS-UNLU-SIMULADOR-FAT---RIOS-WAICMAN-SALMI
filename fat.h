#ifndef FAT_H
#define FAT_H

#define max_sectores 100
#define fin_archivo -1

typedef struct   
    {
    char nombre_archivo [20];
    int bloque_inicio;
    int cantidad_bloques;
    int borrado;
    } Tabla_indice;

typedef struct
    {
    char datos [10];
    int bloque_siguiente;
    } Tabla_datos;

typedef struct
    {
    int ocupado;
    } Tabla_bit;

void inicializar();
void agregar();
void mostrar();
void listar();
void borrar();
#endif
