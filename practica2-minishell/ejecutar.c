#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

#include "parser.h"
#include "ejecutar.h"
#include "libmemoria.h"
#include "redirecciones.h"

int **crear_pipes(int nordenes)
{
    int **pipes = NULL;
    int i;

    // Reservamos memoria para el array de punteros
    pipes = (int **)malloc(sizeof(int *) * (nordenes - 1));
    
    for (i = 0; i < (nordenes - 1); i++)
    {
        // Reservamos memoria para los 2 descriptores de cada pipe
        pipes[i] = (int *)malloc(sizeof(int) * 2);
        
        // Creamos la tubería usando el servicio POSIX pipe() 
        if (pipe(pipes[i]) == -1) {
            perror("Error al crear la tuberia");
            exit(EXIT_FAILURE);
        }
    }
    return pipes;
}

pid_t ejecutar_orden(const char *orden, int entrada, int salida, int *pbackgr)
{
    char **args;
    pid_t pid;
    int indice_ent = -1, indice_sal = -1; /* por defecto, no hay < ni > */

    if ((args = parser_orden(orden, &indice_ent, &indice_sal, pbackgr)) == NULL)
    {
        return(-1);
    }
    if (args[0] == NULL) {
      free_argumentos(args); //Esto funcional tal que si tenemos "" que lo "salte" y sigue haciendo su función
      return -1;
      }
    if (indice_ent != -1) {
      redirec_entrada(args, indice_ent, &entrada);//Para comprobar si parsear encontró redirecciones
      }
    if (indice_sal != -1) {
      redirec_salida(args, indice_sal, &salida);//Para comprobar si parsear encontró redirecciones
      }
      //Si "Open" falló, alguno de los dos es <0
    if (entrada < 0 || salida < 0) {
      free_argumentos(args);
      return -1;
      }
      
    pid = fork();
    if (pid == 0){
      //Si entrada no es =0 redirigimos
        if (entrada != STDIN_FILENO) {
          dup2(entrada, STDIN_FILENO);
          close(entrada);//Cerramos el original
          }
         //Si salida no es =1, redirigimos 
        if (salida != STDOUT_FILENO) {
          dup2(salida, STDOUT_FILENO);
          close(salida);//Cerramos el original
          }
        execvp(args[0], args);
        exit(EXIT_FAILURE);
    }
    else {
      //El padre simplemente cierra los que abrió
        if (entrada != STDIN_FILENO) {
          close(entrada);
          }
        //El padre simplemente cierra los que abrió
        if (salida != STDOUT_FILENO) {
          close(salida);
          }
        free_argumentos(args);//El padre es el ÚNICO que libera, el hijo no
        return pid;
    }
}

void ejecutar_linea_ordenes(const char *orden)
{   
    char *orden_copia; //Crea copia del string original
    char *comando; //Este puntero en cada vuelta apunta a lo que nos interesa
    char *guardar_ptr; //Guarda por donde íbamos
    
    char **ordenes_pipe;
    int nordenes;
    int **pipes = NULL;
    pid_t *pids = NULL; // Array para guardar los PIDs de los hijos
    int backgr = 0;
    int entrada, salida;
    int i;
    
    orden_copia=strdup(orden); //Crea una copia exacta del string orden ya que strtok "crea y destruye"
    for (comando = strtok_r(orden_copia, ";", &guardar_ptr); //Separa y pone en vez del ; un \0, en comando guarda lo de delante y en guardar lo de despúes
        comando != NULL;
        comando = strtok_r(NULL, ";",&guardar_ptr))   //Al empezar con NULL sigue desde donde antes en vez de empezar un nuevo string
        {
        // Obtenemos las órdenes separadas por '|' 
        ordenes_pipe = parser_pipes(comando, &nordenes);
        
        if (nordenes == 0) {
            free(ordenes_pipe);
            continue;
        }

        // Reservamos memoria para los PIDs
        pids = (pid_t *)malloc(sizeof(pid_t) * nordenes);

        // Si hay más de una orden, creamos las tuberías 
        if (nordenes > 1) {
            pipes = crear_pipes(nordenes);
        } else {
            pipes = NULL;
        }

        // Bucle para ejecutar cada orden de la tubería
        for (i = 0; i < nordenes; i++)
        {
            // 1. Configuración de ENTRADA
            if (i == 0) {
                // Primera orden: lee de entrada estándar (o redirección en ejecutar_orden)
                entrada = STDIN_FILENO;
            } else {
                // Órdenes intermedias o final: leen del pipe anterior
                entrada = pipes[i-1][0];
            }

            // 2. Configuración de SALIDA
            if (i == nordenes - 1) {
                // Última orden: escribe en salida estándar (o redirección)
                salida = STDOUT_FILENO;
            } else {
                // Órdenes iniciales o intermedias: escriben en el pipe actual
                salida = pipes[i][1];
            }

            // 3. Ejecución
            // Pasamos 'entrada' y 'salida' a ejecutar_orden
            pids[i] = ejecutar_orden(ordenes_pipe[i], entrada, salida, &backgr);
            
            // El padre cierra los extremos de los pipes que ya no necesita.
            // Si no lo hace, los hijos se quedarán esperando indefinidamente.
            if (i > 0) {
                close(pipes[i-1][0]); // Cierra lectura del anterior
            }
            if (i < nordenes - 1) {
                close(pipes[i][1]);   // Cierra escritura del actual
            }
        }

        // Espera de procesos (Solo si no es background) 
        if (backgr == 0) {
            // Esperamos a todos los hijos de la tubería
            for (i = 0; i < nordenes; i++) {
                if (pids[i] > 0) {
                    waitpid(pids[i], NULL, 0);
                }
            }
        }

        // Liberación de memoria de la tubería actual 
        free_ordenes_pipes(ordenes_pipe, pipes, nordenes);
        free(pids);
    }

        
    free (orden_copia);
}
