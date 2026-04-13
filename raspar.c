#include "raspar.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

void *raspar(void *arg) {
    thread_args *args = (thread_args *) arg;

    for (int i = args->inicio; i < args->fin; i++) {
        int numeros[3];
        for (int j = 0; j < 3; j++) {
            double probabilidad = rand_r(&args->seed) / (double) RAND_MAX;
            numeros[j] = probabilidad > 0.5 ? 5 : 1;

            if (intentos <= MAX_INTENTOS_VISTA) {
                probabilidades[i*3 + j] = probabilidad;
            }
        }

        int resultado = (numeros[0] == numeros[1] && numeros[1] == numeros[2]) ? numeros[0] : 0;
        args->monto_parcial += resultado;

        if (resultado > 0) {
            args->ganadores_parcial++;
            if (resultado == 1) args->ganadores_1_parcial++;
            else args->ganadores_5_parcial++;
        }

        if (intentos <= MAX_INTENTOS_VISTA) {
            numeros_obtenidos[i*3]     = numeros[0];
            numeros_obtenidos[i*3 + 1] = numeros[1];
            numeros_obtenidos[i*3 + 2] = numeros[2];
            resultados[i] = resultado;
        }
    }

    pthread_mutex_lock(&mutex);
    monto_ganado += args->monto_parcial;
    ganadores    += args->ganadores_parcial;
    ganadores_1  += args->ganadores_1_parcial;
    ganadores_5  += args->ganadores_5_parcial;
    pthread_mutex_unlock(&mutex);

    return NULL;
}

void muestreo() {
    srand(time(0));

    if (intentos <= MAX_INTENTOS_VISTA) {
        probabilidades    = calloc(3 * intentos, sizeof(double));
        resultados        = calloc(intentos, sizeof(int));
        numeros_obtenidos = malloc(3 * intentos * sizeof(uint8_t));
    }

    cant_threads = sysconf(_SC_NPROCESSORS_ONLN);
    printf("Cantidad de threads: %d\n", cant_threads);
    if (cant_threads < 1) cant_threads = 1;
    if (cant_threads > intentos) cant_threads = intentos;

    pthread_t threads[cant_threads];
    thread_args args[cant_threads];

    unsigned long long porcion = intentos / cant_threads;
    for (int i = 0; i < cant_threads; i++) {
        args[i].inicio              = i * porcion;
        args[i].fin                 = (i != cant_threads - 1) ? (i + 1) * porcion : intentos;
        args[i].monto_parcial       = 0;
        args[i].ganadores_parcial   = 0;
        args[i].ganadores_1_parcial = 0;
        args[i].ganadores_5_parcial = 0;
        args[i].seed                = time(0) ^ (i * 12345);
        pthread_create(&threads[i], NULL, raspar, &args[i]);
    }

    for (int i = 0; i < cant_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    valor_tarjeta           = monto_ganado / (double) intentos;
    porcentaje_ganadores    = ganadores * 100.0 / intentos;
}

unsigned long long validar_intentos(char *argumento) {
    char *endptr;
    unsigned long long valor = strtoull(argumento, &endptr, 10);

    if (endptr == argumento || *endptr != '\0' || valor == 0) {
        printf("Error. Número de intentos inválido");
        return 0;
    }

    return valor;
}