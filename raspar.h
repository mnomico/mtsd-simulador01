#ifndef RASPAR_H
#define RASPAR_H

#include <ncurses.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>

#define COL 12
#define MAX_INTENTOS_VISTA 1000

extern unsigned long long intentos;
extern double *probabilidades;
extern uint8_t *numeros_obtenidos;
extern int *resultados;
extern long long ganadores, ganadores_1, ganadores_5;
extern double porcentaje_ganadores;
extern unsigned long long monto_ganado;
extern double valor_tarjeta;

extern int cant_threads;
typedef struct {
    int inicio;
    int fin;
    long long monto_parcial;
    long long ganadores_parcial;
    long long ganadores_1_parcial;
    long long ganadores_5_parcial;
    unsigned int seed;
} thread_args;
extern pthread_mutex_t mutex;

void *raspar(void *arg);
void muestreo();
unsigned long long validar_intentos(char *argumento);
void iniciar_vista();

#endif