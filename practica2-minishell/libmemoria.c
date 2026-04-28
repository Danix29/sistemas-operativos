/************************************************************
*  Fichero: libmemoria.c
 *  Autor: Julia Clemente Párraga
 *  Fecha: 2019-2025
 *
 *  Descripción:
 *  Funciones de gestión de memoria dinámica para la minibash.
 *  Permiten liberar arrays de punteros a cadenas utilizados
 *  para almacenar argumentos de órdenes y tuberías.
 ************************************************************/

#include <stdlib.h>

/*-----------------------------------------------------------
 * Funciones públicas
 *----------------------------------------------------------*/

/**
 * @brief Libera un array dinámico de punteros a cadenas.
 *
 * Recorre el array hasta encontrar NULL, liberando cada
 * cadena individual y finalmente el array en sí.
 *
 * @param args Array de cadenas terminado en NULL
 */
void free_argumentos(char **args)
{
    int i = 0;

    if (!args)
        return;

    while (args[i])
        free(args[i++]);
    free(args);
}

/**
 * @brief Libera un array de órdenes y sus pipes asociados.
 *
 * Recorre cada orden y libera su memoria, luego libera
 * los arrays de pipes (fds) asociados y finalmente el array
 * de órdenes.
 *
 * @param ordenes Array de cadenas con órdenes
 * @param fds     Array de arrays de enteros para pipes
 * @param nordenes Número de órdenes / tamaño de los arrays
 */
void free_ordenes_pipes(char **ordenes, int **fds, int nordenes)
{
    int i;

    if (!ordenes)
        return;

    for (i = 0; i < nordenes; i++)
    {
        free(ordenes[i]);
        if (fds && i < (nordenes - 1))
            free(fds[i]);
    }

    free(ordenes);
    if (fds)
        free(fds);
}
