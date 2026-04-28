/************************************************************
*  Fichero: entrada_minishell.c
 *  Autor: Julia Clemente Párraga
 *  Fecha: 2019-2025
 *
 *  Descripción:
 *  Funciones para la entrada de órdenes en la minibash.
 *  Incluye:
 *    - imprimir_prompt(): muestra un prompt sencillo.
 *    - eliminar_salto_linea(): limpia el '\n final de fgets.
 *    - leer_linea_ordenes(): lee una línea de la entrada estándar
 *      y la prepara para ser procesada.
 ************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "entrada_minishell.h"

/*-----------------------------------------------------------
 * Funciones públicas
 *----------------------------------------------------------*/

/**
 * @brief Imprime un prompt sencillo en la consola.
 *
 * Muestra "minishell> " y vacía el buffer de salida para
 * que aparezca inmediatamente.
 */
void imprimir_prompt()
{
    printf("minishell> ");
    fflush(stdout);  // vacía el buffer de salida
}

/**
 * @brief Sustituye el salto de línea al final de una cadena
 *        por el carácter nulo '\0'.
 *
 * @param cad Cadena que será procesada
 */
void eliminar_salto_linea(char *cad)
{
    if (!cad)
        return;

    size_t longitud = strlen(cad);

    if (longitud == 0)
        return;

    // Si el último carácter es '\n', reemplazarlo por '\0'
    if (cad[longitud - 1] == '\n')
        cad[longitud - 1] = '\0';
}

/**
 * @brief Lee una línea de la entrada estándar y la limpia del '\n' final.
 *
 * @param buf Buffer donde se almacenará la línea leída (debe tener tamaño BUFSIZ)
 */
void leer_linea_ordenes(char *buf)
{
    if (!buf)
        return;

    // Inicializar el buffer a cero
    memset(buf, 0, BUFSIZ);

    // Leer la línea
    if (fgets(buf, BUFSIZ - 1, stdin) == NULL)
    {
        buf[0] = '\0';  // Si fgets falla, dejar cadena vacía
        return;
    }

    // Eliminar el salto de línea final
    eliminar_salto_linea(buf);
}
