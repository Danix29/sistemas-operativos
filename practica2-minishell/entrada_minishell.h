/************************************************************
*  Fichero: entrada_minishell.h
 *  Autor: Julia Clemente Párraga
 *  Fecha: 2019-2025
 *
 *  Descripción:
 *  Cabecera del módulo de entrada de órdenes de la minibash.
 *  Declara funciones para mostrar el prompt y leer líneas
 *  de la entrada estándar.
 ************************************************************/

#ifndef ENTRADA_MINISHELL_H
#define ENTRADA_MINISHELL_H

/*-----------------------------------------------------------
 * Funciones públicas
 *----------------------------------------------------------*/

/**
 * @brief Lee una línea de la entrada estándar y elimina
 *        el salto de línea final.
 *
 * @param cad Buffer donde se almacenará la línea leída
 *            (debe tener tamaño suficiente, normalmente BUFSIZ)
 */
void leer_linea_ordenes(char *cad);

/**
 * @brief Muestra un prompt sencillo en pantalla.
 *
 * Imprime "minishell> " y vacía el buffer de salida para
 * que aparezca inmediatamente.
 */
void imprimir_prompt(void);

#endif /* ENTRADA_MINISHELL_H */
