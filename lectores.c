#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lectores.h"

// ---------- LEER HISTORIAL ----------
int leerHistorial(const char *filename, Historial historial[], int max) {
    FILE *historialtxt;
    historialtxt = fopen(filename, "r");
    if (!historialtxt) {
        perror("Error abriendo historial");
        return 0;
    }

    int count = 0;
    char linea[256];
    while (fgets(linea, sizeof(linea), historialtxt) && count < max) {
        linea[strcspn(linea, "\n")] = 0; // quitar salto de línea
        sscanf(linea, "%[^;];%[^;];%[^;];%[^;];%d",
               historial[count].usuario,
               historial[count].tipo,
               historial[count].contenido,
               historial[count].fecha,
               &historial[count].minutos);
        count++;
    }
    fclose(historialtxt);
    return count;
}

// ---------- LEER USUARIOS ----------
int leerUsuarios(const char *filename, Usuario usuarios[], int max) {
    FILE *usuariostxt;
    usuariostxt = fopen(filename, "r");
    if (!usuariostxt) {
        perror("Error abriendo usuarios");
        return 0;
    }

    int count = 0;
    char linea[256];
    while (fgets(linea, sizeof(linea), usuariostxt) && count < max) {
        linea[strcspn(linea, "\n")] = 0;
        sscanf(linea, "%[^;];%[^;];%d;%c;%[^\n]",
               usuarios[count].nombre,
               usuarios[count].nacimiento,
               &usuarios[count].edad,
               &usuarios[count].genero,
               usuarios[count].gustos);
        count++;
    }
    fclose(usuariostxt);
    return count;
}

// ---------- LEER INVENTARIO ----------
int leerInventario(const char *filename, Pelicula peliculas[], int *nPeliculas,
                   Serie series[], int *nSeries) {
    FILE *inventariotxt;
    inventariotxt = fopen(filename, "r");

    char linea[512];
    int modo = 0; // 0=ninguno, 1=peliculas, 2=series
    *nPeliculas = *nSeries = 0;

    while (fgets(linea, sizeof(linea), inventariotxt)) {
        linea[strcspn(linea, "\n")] = 0;

        if (strstr(linea, "=== Peliculas ===")) {   //Modo peliculas
            modo = 1;
            continue;
        }
        if (strstr(linea, "=== Series ===")) {  //Modo series
            modo = 2;
            continue;
        }

        if (modo == 1 && strlen(linea) > 0) {
            sscanf(linea, "%[^;];%d;%[^;];%d;%s",
                   peliculas[*nPeliculas].titulo,
                   &peliculas[*nPeliculas].anio,
                   peliculas[*nPeliculas].genero,
                   &peliculas[*nPeliculas].duracion,
                   peliculas[*nPeliculas].clasificacion);
            (*nPeliculas)++;
        } else if (modo == 2 && strlen(linea) > 0 && linea[0] != ' ') {
            // Solo parseamos la línea principal de la serie
            sscanf(linea, "%[^;];%[^;];%[^;];%d;",
                   series[*nSeries].titulo,
                   series[*nSeries].genero,
                   series[*nSeries].clasificacion,
                   &series[*nSeries].temporadas);
            (*nSeries)++;
        }
    }

    fclose(inventariotxt);
    return 1;
}

// Agregar contenido al inventario (cambiar el fp)
void agregarInventario(const char *filename) {
    FILE *fp = fopen(filename, "a"); // abrimos en append
    if (!fp) {
        perror("Error abriendo inventario");
        return;
    }

    char tipo;
    printf("¿Desea agregar una Pelicula (P) o Serie (S)? ");
    scanf(" %c", &tipo);

    if (tipo == 'P' || tipo == 'p') {
        char titulo[100], genero[50], clasificacion[10];
        int anio, duracion;

        printf("Titulo: ");
        scanf(" %[^\n]", titulo);
        printf("Año: ");
        scanf("%d", &anio);
        printf("Genero: ");
        scanf(" %[^\n]", genero);
        printf("Duracion (minutos): ");
        scanf("%d", &duracion);
        printf("Clasificacion (ej: +13, ATP): ");
        scanf(" %[^\n]", clasificacion);

        fprintf(fp, "\n=== Peliculas ===\n");
        fprintf(fp, "%s;%d;%s;%d;%s\n", titulo, anio, genero, duracion, clasificacion);

    } else if (tipo == 'S' || tipo == 's') {
        char titulo[100], genero[50], clasificacion[10];
        int temporadas;

        printf("Titulo: ");
        scanf(" %[^\n]", titulo);
        printf("Genero: ");
        scanf(" %[^\n]", genero);
        printf("Clasificacion (ej: +13, ATP): ");
        scanf(" %[^\n]", clasificacion);
        printf("Cantidad de temporadas: ");
        scanf("%d", &temporadas);

        fprintf(fp, "\n=== Series ===\n");
        fprintf(fp, "%s;%s;%s;%d;", titulo, genero, clasificacion, temporadas);

        // Escribimos cantidad de capítulos por temporada
        fprintf(fp, "[");
        for (int t = 0; t < temporadas; t++) {
            int capitulos;
            printf("Número de capítulos de la temporada %d: ", t+1);
            scanf("%d", &capitulos);
            fprintf(fp, "%d", capitulos);
            if (t < temporadas - 1) fprintf(fp, ", ");
        }
        fprintf(fp, "]\n");

        // Escribimos la duración de cada capítulo
        for (int t = 0; t < temporadas; t++) {
            int capitulos;
            printf("Ingrese de nuevo la cantidad de capítulos de la temporada %d: ", t+1);
            scanf("%d", &capitulos);

            fprintf(fp, "  Temporada %d: [", t+1);
            for (int c = 0; c < capitulos; c++) {
                int duracion;
                printf("Duracion del capitulo %d de la temporada %d: ", c+1, t+1);
                scanf("%d", &duracion);
                fprintf(fp, "%d", duracion);
                if (c < capitulos - 1) fprintf(fp, ", ");
            }
            fprintf(fp, "]\n");
        }
    } else {
        printf("Opcion no valida.\n");
    }

    fclose(fp);
}

// Buscar películas por título o parte del título (sensible a mayúsculas/minúsculas)
void buscarPelicula(const char *filename, const char *busqueda) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("Error abriendo inventario");
        return;
    }

    char linea[256];
    int enPeliculas = 0;
    int encontrados = 0;

    while (fgets(linea, sizeof(linea), fp)) {
        linea[strcspn(linea, "\n")] = 0; // quitar salto de línea

        if (strstr(linea, "=== Peliculas ===")) {
            enPeliculas = 1;
            continue;
        }
        if (strstr(linea, "=== Series ===")) {
            enPeliculas = 0; // dejamos de leer películas
            continue;
        }

        if (enPeliculas && strlen(linea) > 0) {
            Pelicula p;
            sscanf(linea, "%[^;];%d;%[^;];%d;%s",
                   p.titulo, &p.anio, p.genero, &p.duracion, p.clasificacion);

            // buscamos la subcadena tal cual (case-sensitive)
            if (strstr(p.titulo, busqueda)) {
                printf(">> %s (%d) | Genero: %s | Duracion: %d min | Clasificacion: %s\n",
                       p.titulo, p.anio, p.genero, p.duracion, p.clasificacion);
                encontrados++;
            }
        }
    }

    if (encontrados == 0) {
        printf("No se encontraron peliculas con '%s'\n", busqueda);
    }

    fclose(fp);
}
