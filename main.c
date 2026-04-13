#include "raspar.h"
#include "vista.h"

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

unsigned long long intentos;
double *probabilidades;
uint8_t *numeros_obtenidos;
int *resultados;
long long ganadores, ganadores_1, ganadores_5;
double porcentaje_ganadores;
unsigned long long monto_ganado;
double valor_tarjeta;

int cant_threads;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char *argv[]) {
    int opt;

    if (argc == 1) {
        printf("Uso: %s -m <intentos>\n", argv[0]);
        return 1;
    }

    while((opt = getopt(argc, argv, "m:h")) != -1) {
        switch(opt) {
            case 'm':
                intentos = validar_intentos(optarg);
                break;
            case 'h':
                printf("Uso: %s -m <intentos>\n", argv[0]);
                return 1;
        }
    }

    muestreo();
    iniciar_vista();

    if (intentos <= MAX_INTENTOS_VISTA) {
        free(numeros_obtenidos);
        free(probabilidades);
        free(resultados);
    }
}