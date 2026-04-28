/************************************************************
 *  Fichero: internas.c
 *  Autor: Julia Clemente Párraga
 *  Fecha: 2019-2025
 *
 *  Descripción:
 *  Implementación "básica" de las órdenes internas de la minibash.
 *  Incluye:
 *    - cd     : cambia el directorio de trabajo
 *    - pwd    : muestra el directorio actual
 *    - umask  : consulta o establece la máscara de creación de archivos
 *    - declare: visualiza o establece variables de entorno
 *
 *  Proporciona funciones para:
 *    - detectar si una orden es interna
 *    - ejecutar la orden interna correspondiente
 ************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

#include "internas.h"
#include "parser.h"
#include "libmemoria.h"

/*-----------------------------------------------------------
 * Funciones internas (no visibles fuera de este módulo)
 *----------------------------------------------------------*/

/**
 * @brief Muestra el directorio de trabajo actual.
 */
static void func_pwd(char **args)
{
    char buf[BUFSIZ];

    if (getcwd(buf, sizeof(buf)) == NULL)
        perror("pwd");
    else
        printf("%s\n", buf);
}

/**
 * @brief Cambia el directorio de trabajo.
 *        Si no se pasa argumento, usa HOME.
 */
static void func_cd(char **args)
{
    const char *dir_dest = (args[1] ? args[1] : getenv("HOME"));

    if (!dir_dest)
    {
        fprintf(stderr, "cd: HOME no definido\n");
        return;
    }

    if (chdir(dir_dest) != 0)
        perror("cd");
    else
        func_pwd(NULL);
}

/**
 * @brief Consulta o establece la máscara de creación de archivos.
 *        Sin argumento muestra la máscara actual. Con argumento la cambia.
 */
static void func_umask(char **args)
{
    mode_t antigua, nueva = 0777;

    if (!args[1])
    {
        // Obtener máscara actual sin modificarla
        antigua = umask(nueva);
        umask(antigua);
        printf("%04o\n", antigua);
    }
    else
    {
        char *finptr;
        nueva = strtoul(args[1], &finptr, 8);
        if (*finptr != '\0')
        {
            fprintf(stderr, "umask: argumento inválido\n");
            return;
        }
        umask(nueva);
    }
}

/**
 * @brief Muestra todas las variables de entorno o establece una variable.
 *        Si args[1] es NULL, imprime todas. Si tiene formato VAR=VAL, la establece.
 */
static void func_declare(char **args)
{
    extern char **environ;

    if (!args[1])
    {
        for (int i = 0; environ[i] != NULL; i++)
            printf("%s\n", environ[i]);
        return;
    }

    char *copia = strdup(args[1]);
    if (!copia)
    {
        perror("declare");
        return;
    }

    char *valor = strchr(copia, '=');
    if (!valor || valor == copia)
    {
        fprintf(stderr, "declare: formato inválido, debe ser VAR=VAL\n");
        free(copia);
        return;
    }

    *valor++ = '\0';
    if (setenv(copia, valor, 1) != 0)
        perror("declare");

    free(copia);
}

/*-----------------------------------------------------------
 * Tabla de órdenes internas
 *----------------------------------------------------------*/

static struct tipo_interna internas[] = {
    {"cd", func_cd},
    {"pwd", func_pwd},
    {"umask", func_umask},
    {"declare", func_declare},
    {NULL, NULL} // marca final
};

/*-----------------------------------------------------------
 * Funciones públicas
 *----------------------------------------------------------*/

/**
 * @brief Comprueba si una orden es interna.
 */
int es_ord_interna(const char *buf)
{
    int i, valret = 0;
    int indice_entrada = -1, indice_salida = -1;
    int backgr;
    char **args;

    if ((args = parser_orden(buf, &indice_entrada, &indice_salida, &backgr)) == NULL)
        return 0;

    for (i = 0; internas[i].nombre != NULL; i++)
    {
        if (strcmp(args[0], internas[i].nombre) == 0)
        {
            valret = 1;
            break;
        }
    }

    free_argumentos(args);
    return valret;
}

/**
 * @brief Ejecuta la orden interna correspondiente.
 */
void ejecutar_ord_interna(const char *buf)
{
    int i;
    int indice_entrada = -1, indice_salida = -1;
    int backgr;
    char **args;

    if ((args = parser_orden(buf, &indice_entrada, &indice_salida, &backgr)) == NULL)
        return;

    for (i = 0; internas[i].nombre != NULL; i++)
    {
        if (strcmp(args[0], internas[i].nombre) != 0)
            continue;

        internas[i].func(args);  // ahora pasamos todo el array de argumentos
        break;
    }

    free_argumentos(args);
}
