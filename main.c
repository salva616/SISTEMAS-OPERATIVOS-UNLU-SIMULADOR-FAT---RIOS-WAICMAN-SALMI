#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fat.h"
#include "utils.h"

int main() {
    Sistema_FAT fs;
    inicializar(&fs);
    
    int opcion;
    char nombre[256];
    char contenido[1024];

    do {
        limpiar_pantalla();
        printf("====== SISTEMA DE ARCHIVOS FAT ======\n");
        printf("1. Insertar archivo\n");
        printf("2. Leer archivo\n");
        printf("3. Ver lista de archivos\n");
        printf("4. Borrar archivo\n");
        printf("5. Mostrar fragmentacion (Matriz y Tablas)\n");
        printf("6. Salir\n");
        printf("Seleccione una opcion: ");
        
        if (scanf("%d", &opcion) != 1) {
            limpiar_pantalla();
            printf("Error: Entrada invalida.\n");
            limpiar_buffer();
            esperar_tecla();
            continue;
        }
        limpiar_buffer();
        limpiar_pantalla(); 

        switch(opcion) {
            case 1:
                printf("--- INSERTAR ARCHIVO ---\n");
                printf("Nombre del archivo: ");
                fgets(nombre, sizeof(nombre), stdin);
                nombre[strcspn(nombre, "\n")] = 0; 
                
                printf("Contenido del archivo (String): ");
                fgets(contenido, sizeof(contenido), stdin);
                contenido[strcspn(contenido, "\n")] = 0;
                
                printf("\n");
                agregar(&fs, nombre, contenido);
                esperar_tecla();
                break;
            case 2:
                printf("--- LEER ARCHIVO ---\n");
                printf("Nombre del archivo a leer: ");
                fgets(nombre, sizeof(nombre), stdin);
                nombre[strcspn(nombre, "\n")] = 0;
                
                printf("\n");
                mostrar(&fs, nombre);
                esperar_tecla();
                break;
            case 3:
                listar(&fs);
                esperar_tecla();
                break;
            case 4:
                printf("--- BORRAR ARCHIVO ---\n");
                printf("Nombre del archivo a borrar: ");
                fgets(nombre, sizeof(nombre), stdin);
                nombre[strcspn(nombre, "\n")] = 0;
                
                printf("\n");
                borrar(&fs, nombre);
                esperar_tecla();
                break;
            case 5:
                mostrar_matriz(&fs);
                esperar_tecla();
                break;
            case 6:
                printf("Cerrando sistema...\n");
                break;
            default:
                printf("Opcion invalida.\n");
                esperar_tecla();
        }
    } while (opcion != 6);

    return 0;
}