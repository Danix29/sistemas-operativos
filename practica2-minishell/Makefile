minishell: libmemoria.o entrada_minishell.o ejecutar.o internas.o minishell.o  libshell.a redirecciones.o
	gcc -g -Wall libmemoria.o entrada_minishell.o ejecutar.o  minishell.o redirecciones.o -L./ -lshell -o minishell
	
ejecutar.o: ejecutar.c ejecutar.h
	gcc -g -Wall -c ejecutar.c
	
entrada_minishell.o: entrada_minishell.c entrada_minishell.h
	gcc -g -Wall -c entrada_minishell.c

libmemoria.o: libmemoria.c libmemoria.h
	gcc -g -Wall -c libmemoria.c

minishell.o: minishell.c internas.h ejecutar.h entrada_minishell.h	
	gcc -g -Wall -c minishell.c
	
parser.o: parser.c parser.h
	gcc -g -Wall -c parser.c

internas.o: internas.c internas.h
	gcc -g -Wall -c internas.c

libshell.a: parser.o internas.o 
	ar -rs libshell.a parser.o internas.o
	
redirecciones.o: redirecciones.c redirecciones.h
	gcc -g -Wall -c redirecciones.c
	
.PHONY: clean
clean: 
	rm -f minishell *.o libshell.a

