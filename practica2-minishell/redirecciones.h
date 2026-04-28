/************************************************************
*  Fichero: redirecciones.h
 *  Autor: Julia Clemente Párraga
 *  Fecha: 2011-2025
 *
 *  Descripción:
 *  Cabecera del módulo de redirecciones de la minibash.
 *  Declara funciones para aplicar redirecciones de entrada
 *  y salida en la línea de órdenes.
 ************************************************************/

#ifndef REDIRECCIONES_H
#define REDIRECCIONES_H

/*-----------------------------------------------------------
 * Funciones públicas
 *----------------------------------------------------------*/

/**
 * @brief Aplica redirección de entrada '<'.
 *
 * Abre el archivo indicado en args[indice_entrada+1] y
 * actualiza el descriptor de entrada. Libera y pone a NULL
 * los elementos correspondientes de args.
 *
 * @param args Array de argumentos de la orden
 * @param indice_entrada Índice del token '<' en args
 * @param entrada Puntero al descriptor de entrada
 */
void redirec_entrada(char **args, int indice_entrada, int *entrada);

/**
 * @brief Aplica redirección de salida '>'.
 *
 * Abre o crea el archivo indicado en args[indice_salida+1] en
 * modo escritura y actualiza el descriptor de salida. Libera
 * y pone a NULL los elementos correspondientes de args.
 *
 * @param args Array de argumentos de la orden
 * @param indice_salida Índice del token '>' en args
 * @param salida Puntero al descriptor de salida
 */
void redirec_salida(char **args, int indice_salida, int *salida);

#endif /* REDIRECCIONES_H */
