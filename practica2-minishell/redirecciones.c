#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#include "redirecciones.h"
#include "libmemoria.h"

/* funcion que abre el archivo situado en la posicion indice_entrada+1 */
/* de la orden args y elimina de ella la redireccion completa          */

void redirec_entrada(char **args, int indice_entrada, int *entrada)
{
  // El nombre del archivo está en la posición siguiente al '<' 
    char *fichero = args[indice_entrada + 1];

    // Abrimos el archivo en modo SOLO LECTURA 
    *entrada = open(fichero, O_RDONLY);

    if (*entrada < 0) {
        perror("minishell: Error al abrir archivo de entrada");
    }

    // "Eliminamos" la redirección de 'args'
    // Liberamos la memoria de los strings (parser.c usó strdup)
    free(args[indice_entrada]);
    free(args[indice_entrada + 1]);
    
    // Ponemos NULL para que execvp no los vea 
    args[indice_entrada] = NULL;
    args[indice_entrada + 1] = NULL;

}

/* funcion que abre el archivo situado en la posicion indice_salida+1 */
/* de la orden args y elimina de ella la redireccion completa         */
void redirec_salida(char **args, int indice_salida, int *salida)
{
  // El nombre del archivo está en la posición siguiente al '>' 
    char *fichero = args[indice_salida + 1];

    // Abrimos el archivo: Escritura, Créalo, Trúncalo. Permisos 0666.
    *salida = open(fichero, O_WRONLY | O_CREAT | O_TRUNC, 0666);

    if (*salida < 0) {
        perror("minishell: Error al abrir archivo de salida");
    }

    // "Eliminamos" la redirección de 'args'
    free(args[indice_salida]);
    free(args[indice_salida + 1]);

    args[indice_salida] = NULL;
    args[indice_salida + 1] = NULL;
}
