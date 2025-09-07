#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char titulo[100];
    int año;
    char genero[50];
    int duracion; // en minutos
    char clasificacion[10];
} Pelicula;

typedef struct {
    char titulo[100];
    int año;
    char clasificacion[10];
    int temporadas;
    int capitulos;
    int duracion;
} Serie;