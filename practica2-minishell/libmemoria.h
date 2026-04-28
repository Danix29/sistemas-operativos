/************************************************************
*  Fichero: libmemoria.h
 *  Autor: Julia Clemente Párraga
 *  Fecha: 2019-2025
 *
 *  Descripción:
 *  Cabecera del módulo de gestión de memoria de la minibash.
 *  Declara funciones para liberar arrays dinámicos de cadenas
 *  utilizadas en argumentos de órdenes y tuberías.
 ************************************************************/

#ifndef LIBMEMORIA_H
#define LIBMEMORIA_H

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
void free_argumentos(char **args);

/**
 * @brief Libera un array de órdenes y los pipes asociados.
 *
 * Recorre cada orden y libera su memoria, luego libera
 * los arrays de pipes (fds) asociados y finalmente el array
 * de órdenes.
 *
 * @param ordenes   Array de cadenas con órdenes
 * @param fds       Array de arrays de enteros para pipes
 * @param nordenes  Número de órdenes / tamaño de los arrays
 */
void free_ordenes_pipes(char **ordenes, int **fds, int nordenes);

#endif /* LIBMEMORIA_H */
