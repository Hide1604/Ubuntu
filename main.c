#include <stdio.h>
#include <stdlib.h>
#include <string.h>

main() {
    /* 
    Lectura de los pelis y series ()
    Lectura de los usuarios
    Lectura del historial de cada usuario
    Creacion de las listas  (de pelis, series)
    Creacion de la lista de usuarios
    Creacion de la lista de historiales
    */
    int opcion;
    do{
        fprint("Bienvenido a EDAstreaming\n");
        fprint("Que desea hacer?\n");
        fprint("1. Agregar Pelicula o Serie\n");
        fprint("2. Buscar una Pelicula o Serie\n");
        fprint("3. Estadisticas de la Pelicula o Serie\n");
        fprint("4. Eliminar Pelicula o Serie\n");
        fprint("5. Mostrar Inventario\n");
        fprint("6. Lista por categorias\n");
        fprint("7. Nuevo Usuario\n");
        fprint("8. Buscar por Usuario\n");
        fprint("9. Recomendaciones\n");
        fprint("0. Salir\n");
        scanf("%d", &opcion);
        switch(opcion){
            case 1:
                // Agregar Pelicula o Serie
                break;
            case 2:
                // Buscar una Pelicula o Serie
                break;
            case 3:
                // Estadisticas de la Pelicula o Serie
                break;
            case 4:
                // Eliminar Pelicula o Serie
                break;
            case 5:
                // Mostrar Inventario
                break;
            case 6:
                // Lista por categorias
                break;
            case 7:
                // Nuevo Usuario
                break;
            case 8:
                // Buscar por Usuario
                break;
            case 9:
                // Recomendaciones
                break;
            case 0:
                fprint("Gracias por usar EDAstreaming\n");
                break;
            default:
                fprint("Opcion no valida\n");
        }
    }while(opcion != 0);

    return 0;
}