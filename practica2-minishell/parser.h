/************************************************************
 *  Fichero: parser.h
 *  Autor: Julia Clemente Párraga
 *  Fecha: 2019-2025
 *
 *  Descripción:
 *  Cabecera del módulo de análisis (parser) de la minibash.
 *  Define las funciones y estructuras necesarias para dividir
 *  y analizar una línea de comandos, separando los distintos
 *  argumentos, operadores de redirección y tuberías.
 ************************************************************/

#ifndef PARSER_H
#define PARSER_H

/*-----------------------------------------------------------
 * Enumeración de estados de la máquina de análisis léxico.
 *----------------------------------------------------------*/
enum TEstado {
    INICIO_ARG,   /**< Estado inicial: esperando el comienzo de un argumento */
    ARG           /**< Estado activo: procesando un argumento */
};

/*-----------------------------------------------------------
 * parser_pipes
 *  Divide una línea de comandos con tuberías (|) en varias
 *  órdenes independientes.
 *
 *  @param orden  Cadena con la línea completa de comandos.
 *  @param total  Puntero a entero donde se almacena el número
 *                total de órdenes detectadas.
 *
 *  @return Array dinámico de cadenas, una por cada orden.
 *          Debe liberarse con free() tras su uso.
 *----------------------------------------------------------*/
char **parser_pipes(const char *orden, int *total);

/*-----------------------------------------------------------
 * parser_orden
 *  Convierte una orden simple en un array de argumentos
 *  (terminado en NULL) adecuado para execvp().
 *
 *  Detecta y marca:
 *    - Redirección de entrada:  '<'
 *    - Redirección de salida:   '>', '>>'
 *    - Redirección de error:    '2>'
 *    - Ejecución en background: '&'
 *
 *  @param orden        Cadena con la orden a analizar.
 *  @param indentrada   Índice del token '<' (o -1 si no hay).
 *  @param indsalida    Índice del token '>', '>>' o '2>' (o -1 si no hay).
 *  @param background   Puntero a entero: 1 si hay '&', 0 si no.
 *
 *  @return Array dinámico de cadenas (argumentos), terminado
 *          con NULL. Debe liberarse con free().
 *----------------------------------------------------------*/
char **parser_orden(const char *orden, int *indentrada, int *indsalida, int *background);

#endif /* PARSER_H */

