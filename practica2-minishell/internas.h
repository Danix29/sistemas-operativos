/************************************************************
*  Fichero: internas.h
 *  Autor: Julia Clemente Párraga
 *  Fecha: 2019-2025
 *
 *  Descripción:
 *  Cabecera del módulo de órdenes internas de la minibash.
 *  Define la estructura y funciones necesarias para identificar
 *  y ejecutar órdenes internas como cd, pwd, umask y declare.
 ************************************************************/

#ifndef INTERNAS_H
#define INTERNAS_H

/*-----------------------------------------------------------
 * Estructura tipo_interna
 *----------------------------------------------------------*/

/**
 * @brief Define una orden interna de la minibash.
 *
 * Contiene el nombre de la orden y un puntero a la función
 * que implementa su comportamiento básico.
 */
struct tipo_interna
{
    char *nombre;                       /**< Nombre de la orden interna */
    void (*func)(char **args); /**< Función que ejecuta la orden */
};

/*-----------------------------------------------------------
 * Funciones públicas
 *----------------------------------------------------------*/

/**
 * @brief Comprueba si la orden recibida es una orden interna.
 *
 * @param buf Línea de comando introducida por el usuario
 * @return 1 si es una orden interna, 0 si no lo es
 */
int es_ord_interna(const char *buf);

/**
 * @brief Ejecuta la orden interna correspondiente si existe.
 *
 * @param buf Línea de comando introducida por el usuario
 */
void ejecutar_ord_interna(const char *buf);

#endif /* INTERNAS_H */
