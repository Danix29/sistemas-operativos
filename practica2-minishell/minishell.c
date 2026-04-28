#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

#include "internas.h"
#include "entrada_minishell.h"
#include "ejecutar.h"

static void manejar_sigchild(int signo) {
	int estado;
	waitpid(-1, &estado, WNOHANG); /**
     * Esta línea "recoge" al hijo "zombie" para limpiar su entrada.
     * -1 Recoge a CUALQUIER hijo que haya terminado.
     * &estado Guarda aquí el estado de salida del hijo.
     * WNOHANG Hace que waitpid no bloquee el shell
     * si no hay ningún hijo terminado que recoger.
     */
	}
int main(int argc, char *argv[]){

    char buf[BUFSIZ];
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = manejar_sigchild; //Asigna el manejar_sigchild como manejador de la señal
    sa.sa_flags = SA_NOCLDSTOP | SA_RESTART; //La primera ignora la pausa de un proceso y la segunda es por si se interrumpe el proceso de lectura de la shell que lo repita automáticamente
    sigaction(SIGCHLD, &sa, NULL);   /**
     * Esta línea "instala" o "activa" nuestro manejador, le dice al sistema operativo: "A partir de ahora, cuando 
     * ocurra la señal SIGCHLD (un hijo terminó), ejecuta la
     * configuración que te paso en la variable 'sa'".
     */
	
    while (1)
    {
        imprimir_prompt();
        leer_linea_ordenes(buf);
        if (strcmp(buf, "exit") == 0){
            break;
        }
        else{
            if (es_ord_interna(buf) == 1){
                ejecutar_ord_interna(buf);
            }
            else{
                 ejecutar_linea_ordenes(buf);
            }
        }
    }
    return 0;
}

