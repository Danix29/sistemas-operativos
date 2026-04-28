/************************************************************
 *  Fichero: parser.c
 *  Autor: Julia Clemente Párraga
 *  Fecha: 2019-2025
 *
 *  Descripción:
 *  Implementación de funciones para analizar (parsear) las
 *  órdenes introducidas en la minibash. Convierte una línea
 *  de comandos en estructuras de datos adecuadas para su
 *  ejecución posterior mediante execvp().
 *
 *  Incluye:
 *    - parser_pipes(): divide una línea con tuberías (|).
 *    - parser_orden(): descompone una orden simple en
 *      argumentos, detectando redirecciones (<, >, >>, 2>)
 *      y ejecución en background (&).
 ************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "parser.h"

/*-----------------------------------------------------------
 * parser_pipes
 *  Divide una línea de comandos en órdenes separadas por '|'.
 *  Devuelve un array dinámico de cadenas (una por cada orden)
 *  y almacena en 'total' el número total de órdenes detectadas.
 *----------------------------------------------------------*/
char **parser_pipes(const char *orden, int *total)
{
    char **ordenes = NULL;
    char *cadena = strdup(orden);   // Copia modificable de la orden original
    char *pord = cadena;
    char *pact;
    int nordenes = 0;

    // Separar la cadena por el delimitador '|'
    while ((pact = strsep(&pord, "|")))
    {
        nordenes++;
        ordenes = (char **)realloc(ordenes, nordenes * sizeof(char *));
        ordenes[nordenes - 1] = strdup(pact);
    }

    free(cadena);
    *total = nordenes;
    return ordenes;
}

/*-----------------------------------------------------------
 * parser_orden
 *  Convierte una línea de comandos en un array dinámico de
 *  argumentos terminados en NULL, listo para execvp().
 *
 *  Además detecta:
 *    - Redirecciones de entrada: '<'
 *    - Redirecciones de salida: '>', '>>'
 *    - Redirección de error estándar: '2>'
 *    - Ejecución en background: '&'
 *
 *  Parámetros:
 *    orden        -> cadena de la orden a analizar
 *    indentrada   -> índice del token '<' (si lo hay)
 *    indsalida    -> índice del token '>', '>>' o '2>' (si lo hay)
 *    background   -> indica si la orden termina con '&'
 *----------------------------------------------------------*/
char **parser_orden(const char *orden, int *indentrada, int *indsalida, int *background)
{
    char buf[BUFSIZ];            // Buffer temporal para construir cada argumento
    int estado = INICIO_ARG;     // Estado de la máquina de estados
    int i, j = 0, k = 0;         // i: índice en la cadena; j: índice en buffer; k: número de argumentos
    char **argumentos = NULL;    // Array dinámico de punteros a cadenas (argumentos)

    *indentrada = -1;            // Por defecto, sin redirección de entrada
    *indsalida = -1;             // Por defecto, sin redirección de salida
    *background = 0;             // Por defecto, no en background

    for (i = 0; orden[i] != '\0'; i++)
    {
        char c = orden[i];

        switch (estado)
        {
            case INICIO_ARG:
                if (isspace(c))
                    break; // Ignorar espacios entre argumentos

                estado = ARG;
                j = 0;

                // Detectar operadores de redirección especiales
                if (c == '<' || c == '>')
                {
                    if (c == '>' && orden[i + 1] == '>') // Redirección '>>'
                    {
                        buf[j++] = '>';
                        buf[j++] = '>';
                        i++;
                    }
                    else if (c == '2' && orden[i + 1] == '>') // Redirección '2>'
                    {
                        buf[j++] = '2';
                        buf[j++] = '>';
                        i++;
                    }
                    else
                    {
                        buf[j++] = c; // Redirección simple '<' o '>'
                    }

                    buf[j] = '\0';
                    k++;
                    argumentos = (char **)realloc(argumentos, k * sizeof(char *));
                    argumentos[k - 1] = strdup(buf);

                    // Registrar el índice de redirección
                    if (buf[0] == '<') *indentrada = k - 1;
                    else *indsalida = k - 1;

                    estado = INICIO_ARG;
                }
                else
                {
                    buf[j++] = c; // Primer carácter de un argumento normal
                }
                break;

            case ARG:
                if (isspace(c))
                {
                    // Fin de argumento: guardarlo
                    buf[j] = '\0';
                    k++;
                    argumentos = (char **)realloc(argumentos, k * sizeof(char *));
                    argumentos[k - 1] = strdup(buf);

                    // Registrar si es una redirección
                    if (buf[0] == '<') *indentrada = k - 1;
                    else if (buf[0] == '>') *indsalida = k - 1;
                    else if (strcmp(buf, ">>") == 0 || strcmp(buf, "2>") == 0) *indsalida = k - 1;

                    estado = INICIO_ARG;
                }
                else
                {
                    // Continuar construyendo el argumento
                    if ((c == '>' && buf[0] == '>') || (c == '>' && buf[0] == '2'))
                    {
                        buf[j++] = c;
                        buf[j] = '\0';
                        k++;
                        argumentos = (char **)realloc(argumentos, k * sizeof(char *));
                        argumentos[k - 1] = strdup(buf);
                        *indsalida = k - 1;
                        estado = INICIO_ARG;
                        break;
                    }

                    buf[j++] = c;
                }
                break;
        }
    }

    // Guardar el último argumento si terminó en estado ARG
    if (estado == ARG)
    {
        buf[j] = '\0';
        k++;
        argumentos = (char **)realloc(argumentos, k * sizeof(char *));
        argumentos[k - 1] = strdup(buf);

        if (buf[0] == '<') *indentrada = k - 1;
        else if (buf[0] == '>') *indsalida = k - 1;
        else if (strcmp(buf, ">>") == 0 || strcmp(buf, "2>") == 0) *indsalida = k - 1;
    }

    if (argumentos == NULL)
        return NULL;

    // Comprobar si el último argumento es '&' (background)
    if (k > 0 && strcmp(argumentos[k - 1], "&") == 0)
    {
        free(argumentos[k - 1]);
        argumentos[k - 1] = NULL;
        *background = 1;
    }
    else
    {
        // Asegurar terminación con NULL (requerido por execvp)
        k++;
        argumentos = (char **)realloc(argumentos, k * sizeof(char *));
        argumentos[k - 1] = NULL;
    }

    return argumentos;
}
