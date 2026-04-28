/*
 * Sistemas Operativos
 * PRÁCTICA 3: Programación con Hilos POSIX
 * ALUMNO: Daniel Del Nogal Buchanan
 * Programa que simula una carrera.
 * 1. Gestión de hilos (create, join, exit).
 * 2. Exclusión mutua (Mutex) para pantalla y estructuras de datos compartidas.
 * 3. Evitación de condiciones de carrera.
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#define N_COCHES 8

/* Estructura de datos del coche */
typedef struct {
    int id;
    const char *nombre; /* Usamos const para evitar warnings con literales */
} coche_t;

coche_t Coches[N_COCHES];

coche_t *clasificacion[N_COCHES];

/* Contador compartido de coches que han cruzado la meta */
volatile int coches_llegados = 0;

/* Mutex para proteger la escritura en el array de clasificación*/
pthread_mutex_t mutex_clasificacion = PTHREAD_MUTEX_INITIALIZER;

/* Mutex para sincronizar la salida por pantalla */
pthread_mutex_t mutex_output = PTHREAD_MUTEX_INITIALIZER;


/* Función ejecutada por los hilos */
void *funcion_coche(void *arg)
{
    coche_t *pcoche = (coche_t *)arg;
    int aleatorio;

    /* Inicialización de semilla aleatoria
     * Se suma pthread_self() para garantizar que hilos creados en el mismo
     * segundo tengan semillas distintas. */
    unsigned int semilla = pcoche->id + (unsigned int)time(NULL) + (unsigned int)pthread_self();

    /* Salida por pantalla */
    pthread_mutex_lock(&mutex_output);
    printf("Salida de %s %d\n", pcoche->nombre, pcoche->id);
    fflush(stdout);
    pthread_mutex_unlock(&mutex_output);

    /* Simulación (cálculo re-entrante con rand_r) */
    aleatorio = rand_r(&semilla) % 10 + 1;
    sleep(aleatorio);

    pthread_mutex_lock(&mutex_output);
    printf("Llegada de %s %d\n", pcoche->nombre, pcoche->id);
    pthread_mutex_unlock(&mutex_output);

    /* Bloqueamos para asegurar exclusión mutua al acceder a variables globales */
    pthread_mutex_lock(&mutex_clasificacion);
    
    /* Registramos el coche en la posición que indica el contador */
    clasificacion[coches_llegados] = pcoche;
    
    /* Incrementamos el contador*/
    coches_llegados++;
    
    pthread_mutex_unlock(&mutex_clasificacion);

    /* Finalizamos devolviendo el puntero al coche*/
    pthread_exit((void *)pcoche);
}

int main(void)
{
    pthread_t hilosCoches[N_COCHES];
    int i;
    const char *nombre_base = "Coche"; 

    printf("Se inicia proceso de creacion de hilos...\n\n");
    printf("SALIDA DE COCHES\n");

    for (i = 0; i < N_COCHES; i++) {
        /* Inicialización de la estructura */
        Coches[i].id = i;
        Coches[i].nombre = nombre_base;

        /* Creamos el hilo pasando la dirección de memoria de este coche */
        if (pthread_create(&hilosCoches[i], NULL, funcion_coche, (void *)&Coches[i]) != 0) {
            perror("Error creando hilo");
            exit(EXIT_FAILURE);
        }
    }

    printf("Proceso de creacion de hilos terminado\n\n");

    for (i = 0; i < N_COCHES; i++) {
        /* El hilo principal espera a que termine cada coche para no finalizar el proceso antes */
        if (pthread_join(hilosCoches[i], NULL) != 0) {
            perror("Error en join");
            exit(EXIT_FAILURE);
        }
    }

    printf("Todos los coches han LLEGADO A LA META\n");

    printf("\nCLASIFICACION FINAL:\n");
    for (i = 0; i < N_COCHES; i++) {
        /* Imprimimos el array ordenado  */
        if (clasificacion[i] != NULL) {
            printf("%dº %s %d\n", i + 1, clasificacion[i]->nombre, clasificacion[i]->id);
        }
    }

    /* Liberación de recursos */
    pthread_mutex_destroy(&mutex_output);
    pthread_mutex_destroy(&mutex_clasificacion);

    return 0;
}
